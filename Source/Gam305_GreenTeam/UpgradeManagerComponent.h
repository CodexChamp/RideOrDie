#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "UpgradeTypes.h"
#include "UpgradeManagerComponent.generated.h"

// Broadcast the full def so BP can Break it (BaseValue, Name, etc)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpgradeApplied, FUpgradeDef, UpgradeDef);

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GAM305_GREENTEAM_API UUpgradeManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnUpgradeApplied OnUpgradeApplied;

	UPROPERTY(BlueprintReadOnly)
	TArray<EUpgradeId> OwnedUpgrades;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades|Defs")
	TArray<FUpgradeDef> UpgradePool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades|Stats")
	float MaxHealth = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades|Stats")
	float Damage = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades|Stats")
	float MoveSpeed = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades|Stats")
	float ScoreMultiplier = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrades|Stats")
	float BuffDurationMultiplier = 1.f;

	// Apply by passing the def directly (BP already has it in the card)
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	void ApplyUpgrade(const FUpgradeDef& UpgradeDef);

	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	bool HasUpgrade(EUpgradeId Id) const;
};
