#include "ZombieDirector.h"

#include "Kismet/GameplayStatics.h"
#include "ZombieLODInterface.h"

#include "Algo/Sort.h"

AZombieDirector::AZombieDirector()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AZombieDirector::BeginPlay()
{
	Super::BeginPlay();

	RefreshPlayer();

	if (bAutoDiscoverOnBeginPlay)
	{
		AutoDiscoverZombies();
	}

	// Clamp bad values
	UpdateInterval = FMath::Max(0.05f, UpdateInterval);
	ActiveCap = FMath::Max(1, ActiveCap);
	ActiveRadius = FMath::Max(0.f, ActiveRadius);
	SleepRadius = FMath::Max(ActiveRadius, SleepRadius); // force hysteresis to be sane

	GetWorldTimerManager().SetTimer(
		TimerHandle_Update,
		this,
		&AZombieDirector::TickDirector,
		UpdateInterval,
		true
	);
}

void AZombieDirector::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorldTimerManager().ClearTimer(TimerHandle_Update);
	Super::EndPlay(EndPlayReason);
}

void AZombieDirector::RefreshPlayer()
{
	APawn* P = UGameplayStatics::GetPlayerPawn(this, 0);
	CachedPlayer = P;
}

void AZombieDirector::RegisterZombie(AActor* Zombie)
{
	if (!IsValid(Zombie)) return;

	// Must implement interface
	if (!Zombie->GetClass()->ImplementsInterface(UZombieLODInterface::StaticClass()))
	{
		return;
	}

	// Add unique (manual because TArray<TWeakObjectPtr> doesn’t have AddUnique for raw AActor*)
	for (const TWeakObjectPtr<AActor>& Z : Zombies)
	{
		if (Z.Get() == Zombie)
		{
			return;
		}
	}

	Zombies.Add(Zombie);
}

void AZombieDirector::UnregisterZombie(AActor* Zombie)
{
	if (!IsValid(Zombie)) return;

	for (int32 i = Zombies.Num() - 1; i >= 0; --i)
	{
		if (Zombies[i].Get() == Zombie)
		{
			Zombies.RemoveAtSwap(i);
		}
	}

	ActiveSet.Remove(Zombie);
}

void AZombieDirector::AutoDiscoverZombies()
{
	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsWithInterface(this, UZombieLODInterface::StaticClass(), Found);

	for (AActor* A : Found)
	{
		RegisterZombie(A);
	}
}

void AZombieDirector::CleanupDeadRefs()
{
	for (int32 i = Zombies.Num() - 1; i >= 0; --i)
	{
		if (!Zombies[i].IsValid())
		{
			Zombies.RemoveAtSwap(i);
		}
	}

	// Clean ActiveSet too
	for (auto It = ActiveSet.CreateIterator(); It; ++It)
	{
		if (!It->IsValid())
		{
			It.RemoveCurrent();
		}
	}
}

void AZombieDirector::SetZombieActiveState(AActor* Zombie, bool bActive)
{
	if (!IsValid(Zombie)) return;
	if (!Zombie->GetClass()->ImplementsInterface(UZombieLODInterface::StaticClass())) return;

	// Always call. The BP side already guards against duplicate work.
	IZombieLODInterface::Execute_SetBrainActive(Zombie, bActive);
}

void AZombieDirector::TickDirector()
{
	if (!CachedPlayer.IsValid())
	{
		RefreshPlayer();
		if (!CachedPlayer.IsValid())
		{
			return;
		}
	}

	CleanupDeadRefs();

	const FVector PlayerLoc = CachedPlayer->GetActorLocation();
	const float ActiveRadiusSq = ActiveRadius * ActiveRadius;
	const float SleepRadiusSq = SleepRadius * SleepRadius;

	struct FZombieDist
	{
		TWeakObjectPtr<AActor> Zombie;
		float DistSq = 0.f;
	};

	TArray<FZombieDist> Candidates;
	Candidates.Reserve(Zombies.Num());

	// Build candidates list (within SleepRadius, otherwise force sleep)
	for (const TWeakObjectPtr<AActor>& ZW : Zombies)
	{
		AActor* Z = ZW.Get();
		if (!IsValid(Z)) continue;

		const float DistSq = FVector::DistSquared(Z->GetActorLocation(), PlayerLoc);

		// If far beyond sleep radius, slam asleep and skip any more work
		if (DistSq >= SleepRadiusSq)
		{
			SetZombieActiveState(Z, false);
			UE_LOG(LogTemp, Warning, TEXT("SLEEP: %s"), *GetNameSafe(Z));

			continue;
		}

		// Within sleep radius: eligible for being active (if also within ActiveRadius)
		Candidates.Add({ ZW, DistSq });
	}

	// Sort by closest
	Algo::Sort(Candidates, [](const FZombieDist& A, const FZombieDist& B)
		{
			return A.DistSq < B.DistSq;
		});

	int32 Activated = 0;

	// Decide active set
	for (const FZombieDist& Entry : Candidates)
	{
		AActor* Z = Entry.Zombie.Get();
		if (!IsValid(Z)) continue;

		const bool bInsideActive = (Entry.DistSq <= ActiveRadiusSq);

		if (!bInsideActive)
		{
			// In the band between ActiveRadius..SleepRadius: leave state alone (hysteresis)
			continue;
		}

		if (bUseClosestCap)
		{
			if (Activated < ActiveCap)
			{
				SetZombieActiveState(Z, true);
				Activated++;
			}
			else
			{
				// Too many: keep it asleep (or force asleep)
				SetZombieActiveState(Z, false);
			}
		}
		else
		{
			SetZombieActiveState(Z, true);
		}
	}

	// If using closest-cap, we already forced extra-inside-radius zombies asleep above.
	// Zombies in the band (ActiveRadius..SleepRadius) keep prior state (prevents flicker).
}
