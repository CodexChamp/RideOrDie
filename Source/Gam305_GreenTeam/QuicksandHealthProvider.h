#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuicksandHealthProvider.generated.h"

UINTERFACE(BlueprintType)
class UQuicksandHealthProvider : public UInterface
{
	GENERATED_BODY()
};

class IQuicksandHealthProvider
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Quicksand|Health")
	float QS_GetMaxHealth() const;
};
