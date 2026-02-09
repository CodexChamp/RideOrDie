#pragma once

#include "CoreMinimal.h"
#include "LeaderboardRuntimeTypes.generated.h"

USTRUCT(BlueprintType)
struct GAM305_GREENTEAM_API FLeaderboardRuntimeEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float TimeSeconds = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Highscore = 0.f;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Level = 0;
};
