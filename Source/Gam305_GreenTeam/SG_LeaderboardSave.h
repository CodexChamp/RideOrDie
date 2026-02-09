#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LeaderboardTypes.h"
#include "SG_LeaderboardSave.generated.h"

UCLASS()
class GAM305_GREENTEAM_API USG_LeaderboardSave : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FLeaderboardEntry> Entries;
};
