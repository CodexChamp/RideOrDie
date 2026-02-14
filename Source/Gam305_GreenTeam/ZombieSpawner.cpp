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

	// ---- Read CurrentLvl dynamically from pawn (BP variable is fine) ----
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
	// SpawnPerWave (default 2) * 2^(Level-1)
	int32 DesiredSpawnPerWave = SpawnPerWave * (1 << (Level - 1));
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
	for (int32 i = AliveZombies.Num() - 1; i >= 0; --i)
	{
		if (!AliveZombies[i].IsValid())
		{
			AliveZombies.RemoveAtSwap(i, 1, false);
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

	// ---- Direction bias: spawn mostly behind the pawn ----
	const FVector Forward = PlayerPawn->GetActorForwardVector().GetSafeNormal();
	const FVector Back = (-Forward).GetSafeNormal();

	const float Distance = FMath::FRandRange(SpawnRadiusMin, SpawnRadiusMax);

	// random yaw offset inside a cone around Back
	const float OffsetDeg = FMath::FRandRange(-BackConeHalfAngleDeg, BackConeHalfAngleDeg);
	const FRotator YawRot(0.f, OffsetDeg, 0.f);

	FVector Dir = YawRot.RotateVector(Back);

	// optional side spread for flanking
	const FVector Right = FVector::CrossProduct(FVector::UpVector, Back).GetSafeNormal();
	const float SideSign = (FMath::FRand() < 0.5f) ? -1.f : 1.f;
	Dir = (Dir + Right * SideSign * SideBias).GetSafeNormal();

	const FVector Candidate = PlayerLoc + (Dir * Distance);

	// ---- Grounding fix ----
	// Visibility often isn't blocked by Landscape. Use WorldStatic and a larger vertical span.
	const FVector Start = Candidate + FVector(0.f, 0.f, 5000.f);
	const FVector End = Candidate - FVector(0.f, 0.f, 10000.f);

	FCollisionQueryParams Params(SCENE_QUERY_STAT(ZombieSpawnTrace), false);
	Params.AddIgnoredActor(PlayerPawn);

	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldStatic, Params))
	{
		// Lift so we don't start intersecting terrain
		const float Lift = 120.f; // bump to 200 if they still clip
		OutLocation = Hit.ImpactPoint + FVector(0.f, 0.f, Lift);
		return true;
	}

	// If trace fails, don't spawn (prevents underground spawns)
	return false;
}
