#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LeaderboardRuntimeTypes.h"
#include "SG_LeaderboardSave_Cpp.generated.h"

UCLASS()
class GAM305_GREENTEAM_API USG_LeaderboardSave_Cpp : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FLeaderboardRuntimeEntry> Entries;
};
