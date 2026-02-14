#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuicksandHealthProvider.h"
#include "QuicksandVolume.generated.h"

class UBoxComponent;

USTRUCT()
struct FQuicksandVictimState
{
	GENERATED_BODY()

	UPROPERTY() TWeakObjectPtr<UPrimitiveComponent> RootPrim;
	UPROPERTY() float Sink01 = 0.f;
	UPROPERTY() bool bKilled = false;

	// store original damping so we restore correctly
	UPROPERTY() float OriginalLinearDamping = 0.f;
	UPROPERTY() float OriginalAngularDamping = 0.f;
	UPROPERTY() bool bHasOriginalDamping = false;
};

UCLASS()
class GAM305_GREENTEAM_API AQuicksandVolume : public AActor
{
	GENERATED_BODY()

public:
	AQuicksandVolume();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Quicksand")
	UBoxComponent* Box;

	// ---------------- Feel knobs ----------------

	// Damping to force a sticky feel (vehicle still drives, but "muddy")
	UPROPERTY(EditAnywhere, Category = "Quicksand|Slow")
	float LinearDampingInSand = 12.0f;

	UPROPERTY(EditAnywhere, Category = "Quicksand|Slow")
	float AngularDampingInSand = 6.0f;

	// Force = -DragK * |V| * V  (quadratic drag)
	UPROPERTY(EditAnywhere, Category = "Quicksand|Slow")
	float DragK = 70.0f;

	// Extra drag multiplier as you sink
	UPROPERTY(EditAnywhere, Category = "Quicksand|Slow")
	float DragBySinkMultiplier = 5.0f;

	// Sink progress speed (0..1 per sec)
	UPROPERTY(EditAnywhere, Category = "Quicksand|Sink")
	float SinkRate01PerSec = 0.25f;

	// Down force strength (mass-aware because we apply as acceleration below)
	UPROPERTY(EditAnywhere, Category = "Quicksand|Sink")
	float DownForceStrength = 220000.0f;

	// Cap upward velocity while inside quicksand so Chaos never yeets you
	UPROPERTY(EditAnywhere, Category = "Quicksand|Sink")
	float MaxUpwardSpeedInSand = 80.0f; // cm/s

	// When Sink01 reaches this, we kill (damage = max health)
	UPROPERTY(EditAnywhere, Category = "Quicksand|Damage")
	float KillSink01 = 0.98f;

	UPROPERTY(EditAnywhere, Category = "Quicksand|Damage")
	TSubclassOf<UDamageType> DamageType;

private:
	UPROPERTY() TMap<TWeakObjectPtr<AActor>, FQuicksandVictimState> Victims;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& Sweep);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* Overlapped, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	static UPrimitiveComponent* GetRootPrimitive(AActor* Actor);
	void ApplyQuicksandToVictim(AActor* Victim, FQuicksandVictimState& State, float Dt);
	void RestoreIfLeaving(FQuicksandVictimState& State);
};
