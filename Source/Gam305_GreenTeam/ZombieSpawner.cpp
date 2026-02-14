#include "ZombieSpawner.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "GameFramework/Pawn.h"

AZombieSpawner::AZombieSpawner()
{
	PrimaryActorTick.bCanEverTick = false; // timer-based, no Tick
}

void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnRadiusMax < SpawnRadiusMin)
	{
		Swap(SpawnRadiusMin, SpawnRadiusMax);
	}

	if (SpawnEverySeconds > 0.f)
	{
		GetWorldTimerManager().SetTimer(
			SpawnTimer,
			this,
			&AZombieSpawner::TrySpawnWave,
			SpawnEverySeconds,
			true
		);
	}
}

void AZombieSpawner::TrySpawnWave()
{
	if (!ZombieClass)
		return;

	CompactAliveList();

	UWorld* World = GetWorld();
	if (!World)
		return;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!PlayerPawn)
		return;

	// ---- Read CurrentLvl dynamically from pawn ----
	int32 Level = 1;

	static const FName CurrentLvlName(TEXT("CurrentLvl"));
	if (FProperty* Prop = PlayerPawn->GetClass()->FindPropertyByName(CurrentLvlName))
	{
		if (FIntProperty* IntProp = CastField<FIntProperty>(Prop))
		{
			Level = IntProp->GetPropertyValue_InContainer(PlayerPawn);
		}
	}

	Level = FMath::Max(Level, 1);

	// ---- Exponential SpawnPerWave ----
	int32 DesiredSpawnPerWave = SpawnPerWave + Level;
	DesiredSpawnPerWave = FMath::Clamp(DesiredSpawnPerWave, 1, MaxSpawnPerWave);


	// ---- Linear MaxAlive scaling ----
	int32 DesiredMaxAlive = MaxAlive + (MaxAlivePerLevel * (Level - 1));
	DesiredMaxAlive = FMath::Max(DesiredMaxAlive, 1);

	if (AliveZombies.Num() >= DesiredMaxAlive)
		return;

	const int32 Room = DesiredMaxAlive - AliveZombies.Num();
	const int32 ToSpawn = FMath::Min(DesiredSpawnPerWave, Room);

	for (int32 i = 0; i < ToSpawn; ++i)
	{
		SpawnOneZombie();
	}
}

bool AZombieSpawner::SpawnOneZombie()
{
	FVector SpawnLocation;
	if (!GetSpawnLocationAroundPlayer(SpawnLocation))
		return false;

	const FRotator SpawnRotation(0.f, FMath::FRandRange(0.f, 360.f), 0.f);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* Spawned = GetWorld()->SpawnActor<APawn>(
		ZombieClass,
		SpawnLocation,
		SpawnRotation,
		Params
	);

	if (!Spawned)
		return false;

	AliveZombies.Add(Spawned);
	return true;
}

void AZombieSpawner::CompactAliveList()
{
	UWorld* World = GetWorld();
	APawn* PlayerPawn = World ? UGameplayStatics::GetPlayerPawn(World, 0) : nullptr;

	// Despawn threshold: 2x max spawn radius from player/car
	const float DespawnDist = SpawnRadiusMax * FMath::Max(DespawnDistanceMultiplier, 0.01f);
	const float DespawnDistSq = DespawnDist * DespawnDist;


	const FVector PlayerLoc = PlayerPawn ? PlayerPawn->GetActorLocation() : FVector::ZeroVector;

	for (int32 i = AliveZombies.Num() - 1; i >= 0; --i)
	{
		APawn* Z = AliveZombies[i].Get();

		// invalid pointer -> remove
		if (!Z)
		{
			AliveZombies.RemoveAtSwap(i, 1, false);
			continue;
		}

		// if we have a player, cull far zombies
		if (PlayerPawn)
		{
			const float DistSq = FVector::DistSquared(PlayerLoc, Z->GetActorLocation());
			if (DistSq > DespawnDistSq)
			{
				Z->Destroy();
				AliveZombies.RemoveAtSwap(i, 1, false);
				continue;
			}
		}
	}
}

bool AZombieSpawner::GetSpawnLocationAroundPlayer(FVector& OutLocation) const
{
	UWorld* World = GetWorld();
	if (!World)
		return false;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(World, 0);
	if (!PlayerPawn)
		return false;

	const FVector PlayerLoc = PlayerPawn->GetActorLocation();

	// ---- Direction bias: mostly in FRONT of the pawn ----
	const FVector Forward = PlayerPawn->GetActorForwardVector().GetSafeNormal();
	const FVector Back = (-Forward).GetSafeNormal();

	const float FrontChance = 0.80f; // 80% front spawn, 20% back spawn
	const bool bFront = (FMath::FRand() < FrontChance);
	const FVector BaseDir = bFront ? Forward : Back;

	const float Distance = FMath::FRandRange(SpawnRadiusMin, SpawnRadiusMax);

	// random yaw offset inside a cone
	const float OffsetDeg = FMath::FRandRange(-BackConeHalfAngleDeg, BackConeHalfAngleDeg);
	const FRotator YawRot(0.f, OffsetDeg, 0.f);

	FVector Dir = YawRot.RotateVector(BaseDir);

	// optional side spread for flanking
	const FVector Right = FVector::CrossProduct(FVector::UpVector, BaseDir).GetSafeNormal();
	const float SideSign = (FMath::FRand() < 0.5f) ? -1.f : 1.f;
	Dir = (Dir + Right * SideSign * SideBias).GetSafeNormal();

	const FVector Candidate = PlayerLoc + (Dir * Distance);

	// ---- Grounding fix ----
	const FVector Start = Candidate + FVector(0.f, 0.f, 5000.f);
	const FVector End = Candidate - FVector(0.f, 0.f, 10000.f);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ZombieSpawnTrace), false);
	Params.AddIgnoredActor(PlayerPawn);

	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		const float Lift = 120.f;
		OutLocation = Hit.ImpactPoint + FVector(0.f, 0.f, Lift);
		return true;
	}

	return false;
}
