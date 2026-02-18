// WBP_LeaderboardRow_Cpp.cpp
#include "WBP_LeaderboardRow_Cpp.h"

#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "RideOrDieGameState.h"

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

	// Highlight the MOST RECENTLY SUBMITTED run (not 1st place)
	bool bHighlightRecent = false;

	if (UWorld* World = GetWorld())
	{
		if (ARideOrDieGameState* GS = World->GetGameState<ARideOrDieGameState>())
		{
			bHighlightRecent = GS->IsLastSubmittedEntry(Entry);
		}
	}

	if (Border_BG)
	{
		Border_BG->SetBrushColor(
			bHighlightRecent
			? FLinearColor(1.f, 0.9f, 0.2f, 0.35f)   // yellow tint
			: FLinearColor(1.f, 1.f, 1.f, 0.1f)     // normal
		);
	}
}
