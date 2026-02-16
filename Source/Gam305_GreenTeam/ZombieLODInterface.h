#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ZombieLODInterface.generated.h"

UINTERFACE(BlueprintType)
class GAM305_GREENTEAM_API UZombieLODInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Implement this on any zombie Blueprint (BP_BasicZombie, BP_ExplodingZombie).
 * Director will call SetBrainActive(true/false) to enable/disable expensive AI.
 */
class GAM305_GREENTEAM_API IZombieLODInterface
{
	GENERATED_BODY()

public:
	/** Turn heavy AI on/off (BT/Perception/MoveTo/Attack loops/etc). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Zombie|LOD")
	void SetBrainActive(bool bActive);

	/** Optional: return current state (Director can still function without it). */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Zombie|LOD")
	bool IsBrainActive() const;
};
