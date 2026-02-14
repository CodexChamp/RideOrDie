#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

class APawn;

UCLASS()
class GAM305_GREENTEAM_API AZombieSpawner : public AActor
{
	GENERATED_BODY()

public:
	AZombieSpawner();

protected:
	virtual void BeginPlay() override;

	// ---- Spawn Settings ----

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnRadiusMin = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnRadiusMax = 5000.f;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	float SpawnEverySeconds = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	int32 MaxAlive = 15;

	// Level 1 spawn count baseline (your exponential uses this)
	UPROPERTY(EditAnywhere, Category = "Spawner")
	int32 SpawnPerWave = 2;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	TSubclassOf<APawn> ZombieClass;

	// ---- Scaling ----

	UPROPERTY(EditAnywhere, Category = "Scaling")
	int32 MaxSpawnPerWave = 64; // safety clamp for exponential

	UPROPERTY(EditAnywhere, Category = "Scaling")
	int32 MaxAlivePerLevel = 5; // linear growth so you don't melt CPU

	// ---- Despawn ----

	// Zombies are destroyed if they are farther than (SpawnRadiusMax * DespawnDistanceMultiplier) from the player.
	UPROPERTY(EditAnywhere, Category = "Despawn", meta = (ClampMin = "0.5", ClampMax = "10.0"))
	float DespawnDistanceMultiplier = 2.0f;

	// ---- Spawn Direction Bias ----

	UPROPERTY(EditAnywhere, Category = "Direction")
	float BackConeHalfAngleDeg = 70.f; // ± degrees around BaseDir

	UPROPERTY(EditAnywhere, Category = "Direction")
	float SideBias = 0.35f; // 0 = straight, ~0.25-0.45 feels good

	// ---- Runtime ----

	UPROPERTY(VisibleAnywhere, Category = "Runtime")
	TArray<TWeakObjectPtr<APawn>> AliveZombies;

private:
	FTimerHandle SpawnTimer;

	void TrySpawnWave();
	bool SpawnOneZombie();
	void CompactAliveList();
	bool GetSpawnLocationAroundPlayer(FVector& OutLocation) const;
};
