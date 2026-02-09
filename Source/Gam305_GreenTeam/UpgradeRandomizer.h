// UpgradeRandomizer.h
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UpgradeTypes.h"
#include "UpgradeRandomizer.generated.h"

UCLASS()
class GAM305_GREENTEAM_API UUpgradeRandomizer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Picks Count UNIQUE upgrades from Pool (weighted).
	// ExcludeIds lets you prevent offering already-owned upgrades.
	UFUNCTION(BlueprintCallable, Category = "Upgrades")
	static TArray<FUpgradeDef> PickRandomUpgrades(
		const TArray<FUpgradeDef>& Pool,
		int32 Count,
		const TArray<EUpgradeId>& ExcludeIds,
		int32 Seed = 0
	);
};
