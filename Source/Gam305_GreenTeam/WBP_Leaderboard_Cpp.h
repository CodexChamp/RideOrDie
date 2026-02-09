// WBP_Leaderboard_Cpp.h
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardRuntimeTypes.h"
#include "WBP_Leaderboard_Cpp.generated.h"

class UVerticalBox;

UCLASS()
class GAM305_GREENTEAM_API UWBP_Leaderboard_Cpp : public UUserWidget
{
	GENERATED_BODY()

public:
	// Call this from BP (or from your HandleLeaderboardChanged)
	UFUNCTION(BlueprintCallable)
	void RebuildUI(const TArray<FLeaderboardRuntimeEntry>& Entries);

protected:
	// VerticalBox in WBP_Leaderboard must be named RowsBox and "Is Variable" checked
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* RowsBox = nullptr;

	// Set this in WBP_Leaderboard -> Class Defaults to WBP_LeaderboardRow
	// (That widget should have Parent Class = UWBP_LeaderboardRow_Cpp)
	UPROPERTY(EditDefaultsOnly, Category = "Leaderboard")
	TSubclassOf<UUserWidget> RowWidgetClass;
};
