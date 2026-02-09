#pragma once

#include "CoreMinimal.h"
#include "LeaderboardTypes.generated.h"

USTRUCT(BlueprintType)
struct FLeaderboardEntry
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString PlayerName;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int32 Score = 0;

    // tiebreaker / debug
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int64 UnixTime = 0;
};
