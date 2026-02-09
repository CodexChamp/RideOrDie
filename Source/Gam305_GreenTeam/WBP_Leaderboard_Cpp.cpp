// WBP_Leaderboard_Cpp.cpp
#include "WBP_Leaderboard_Cpp.h"

#include "Algo/Sort.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Engine/Engine.h"

// FIXME: replace with the REAL header name/path you have in Solution Explorer
#include "WBP_LeaderboardRow_Cpp.h"

static void Screen(const FString& Msg, float Time = 6.f)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Time, FColor::Yellow, Msg);
	}
}

void UWBP_Leaderboard_Cpp::RebuildUI(const TArray<FLeaderboardRuntimeEntry>& Entries)
{
	Screen(FString::Printf(TEXT("RebuildUI CALLED_Entries=%d"), Entries.Num()));
	Screen(FString::Printf(TEXT("RowWidgetClass=%s"), RowWidgetClass ? TEXT("OK") : TEXT("NULL")));
	Screen(FString::Printf(TEXT("RowsBox=%s"), RowsBox ? TEXT("OK") : TEXT("NULL")));

	if (!RowsBox || !RowWidgetClass)
		return;

	TArray<FLeaderboardRuntimeEntry> Working = Entries;
	Algo::Sort(Working, [](const FLeaderboardRuntimeEntry& A, const FLeaderboardRuntimeEntry& B)
		{
			if (A.Highscore != B.Highscore)
				return A.Highscore > B.Highscore;
			return A.TimeSeconds < B.TimeSeconds;
		});

	RowsBox->ClearChildren();

	int32 AddedCount = 0;
	int32 CreateFailCount = 0;
	int32 WrongClassCount = 0;
	int32 NoWorldCount = 0;

	for (int32 i = 0; i < Working.Num(); i++)
	{
		UWorld* World = GetWorld();
		if (!World)
		{
			NoWorldCount++;
			break;
		}

		UUserWidget* Row = CreateWidget<UUserWidget>(World, RowWidgetClass);
		if (!Row)
		{
			CreateFailCount++;
			continue;
		}

		RowsBox->AddChild(Row);
		AddedCount++;

		if (UWBP_LeaderboardRow_Cpp* RowCpp = Cast<UWBP_LeaderboardRow_Cpp>(Row))
		{
			RowCpp->SetRowData(Working[i], i + 1);
		}
		else
		{
			WrongClassCount++;
		}
	}

	Screen(FString::Printf(TEXT("RebuildUI DONE. Added=%d CreateFail=%d WrongClass=%d NoWorld=%d"),
		AddedCount, CreateFailCount, WrongClassCount, NoWorldCount));
}
