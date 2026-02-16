#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieDirector.generated.h"

UCLASS()
class GAM305_GREENTEAM_API AZombieDirector : public AActor
{
	GENERATED_BODY()

public:
	AZombieDirector();

	// ---- Registration (call from BP_BasicZombie BeginPlay / EndPlay, or spawner) ----
	UFUNCTION(BlueprintCallable, Category = "Zombie|Director")
	void RegisterZombie(AActor* Zombie);

	UFUNCTION(BlueprintCallable, Category = "Zombie|Director")
	void UnregisterZombie(AActor* Zombie);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// ---- Tuning ----

	/** How often we update LOD states (seconds). */
	UPROPERTY(EditAnywhere, Category = "Zombie|Director")
	float UpdateInterval = 0.25f;

	/** Hard cap: only this many zombies get "full brain" at once. */
	UPROPERTY(EditAnywhere, Category = "Zombie|Director")
	int32 ActiveCap = 18;

	/** Zombies <= this distance may be considered for activation. */
	UPROPERTY(EditAnywhere, Category = "Zombie|Director")
	float ActiveRadius = 6000.f;

	/** Zombies >= this distance will be forced asleep. (Hysteresis to prevent flicker) */
	UPROPERTY(EditAnywhere, Category = "Zombie|Director")
	float SleepRadius = 9000.f;

	/**
	 * If true: pick closest ActiveCap zombies inside ActiveRadius.
	 * If false: activate everyone inside ActiveRadius (still uses SleepRadius for off).
	 */
	UPROPERTY(EditAnywhere, Category = "Zombie|Director")
	bool bUseClosestCap = true;

	/**
	 * If true: Director will do a one-time world scan on BeginPlay and register anything
	 * implementing ZombieLODInterface. Still recommended to register explicitly for best perf.
	 */
	UPROPERTY(EditAnywhere, Category = "Zombie|Director")
	bool bAutoDiscoverOnBeginPlay = true;

private:
	FTimerHandle TimerHandle_Update;

	// Weak refs so destroyed zombies don't keep hard references.
	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<AActor>> Zombies;

	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> CachedPlayer;

	// Track who is currently active to avoid spamming interface calls.
	UPROPERTY(Transient)
	TSet<TWeakObjectPtr<AActor>> ActiveSet;

	void TickDirector();
	void RefreshPlayer();
	void AutoDiscoverZombies();
	void CleanupDeadRefs();

	void SetZombieActiveState(AActor* Zombie, bool bActive);
};
