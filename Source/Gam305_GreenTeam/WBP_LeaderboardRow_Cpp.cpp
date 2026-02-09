#include "WBP_LeaderboardRow_Cpp.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"

void UWBP_LeaderboardRow_Cpp::SetRowData(const FLeaderboardRuntimeEntry& Entry, int32 PlaceIn)
{
	if (TxtPlace)
	{
		TxtPlace->SetText(FText::AsNumber(PlaceIn));
	}

	if (TxtName)
	{
		TxtName->SetText(FText::FromString(Entry.Name));
	}

	if (TxtTime)
	{
		// show seconds with 2 decimals
		TxtTime->SetText(FText::FromString(
			FString::Printf(TEXT("%.2f"), Entry.TimeSeconds)
		));
	}

	if (TxtLevel)
	{
		TxtLevel->SetText(FText::AsNumber(Entry.Level));
	}

	if (TxtScore)
	{
		TxtScore->SetText(FText::AsNumber(FMath::RoundToInt(Entry.Highscore)));
	}

	// Optional highlight for top entry
	if (Border_BG)
	{
		Border_BG->SetBrushColor(
			PlaceIn == 1 ? FLinearColor(1.f, 0.9f, 0.2f, 0.35f)
			: FLinearColor(1.f, 1.f, 1.f, 0.1f)
		);
	}
}
