#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LeaderboardRuntimeTypes.h"
#include "WBP_LeaderboardRow_Cpp.generated.h"

class UTextBlock;
class UBorder;

UCLASS()
class GAM305_GREENTEAM_API UWBP_LeaderboardRow_Cpp : public UUserWidget
{
	GENERATED_BODY()

public:
	// Called by leaderboard C++
	UFUNCTION(BlueprintCallable)
	void SetRowData(const FLeaderboardRuntimeEntry& Entry, int32 PlaceIn);

protected:
	// Bind these to widgets in WBP_LeaderboardRow
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtPlace = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtTime = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtLevel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TxtScore = nullptr;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* Border_BG = nullptr;
};
