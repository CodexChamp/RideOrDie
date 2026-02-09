// RideOrDieGameState.cpp
#include "RideOrDieGameState.h"

#include "Kismet/GameplayStatics.h"
#include "SG_LeaderboardSave_Cpp.h"

const FString ARideOrDieGameState::SaveSlot = TEXT("LeaderboardSaveCpp");

ARideOrDieGameState::ARideOrDieGameState()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = true;
}

void ARideOrDieGameState::BeginPlay()
{
    Super::BeginPlay();
    LoadLeaderboard();
}

void ARideOrDieGameState::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (!bRunActive)
        return;

    // Uses game time (respects pause/time dilation)
    const float Now = GetWorld() ? GetWorld()->GetTimeSeconds() : 0.f;
    CurrentRunTimeSeconds = FMath::Max(0.f, Now - RunStartTimeSeconds);
}

const TArray<FLeaderboardRuntimeEntry>& ARideOrDieGameState::GetLeaderboard() const
{
    return Leaderboard;
}

// ---------------- Timer API ----------------

void ARideOrDieGameState::StartRunTimer()
{
    if (!GetWorld())
        return;

    bRunActive = true;
    RunStartTimeSeconds = GetWorld()->GetTimeSeconds();
    CurrentRunTimeSeconds = 0.f;
}

void ARideOrDieGameState::StopRunTimer()
{
    bRunActive = false;
}

void ARideOrDieGameState::ResetRunTimer()
{
    bRunActive = false;
    RunStartTimeSeconds = 0.f;
    CurrentRunTimeSeconds = 0.f;
}

float ARideOrDieGameState::GetCurrentRunTime() const
{
    return CurrentRunTimeSeconds;
}

bool ARideOrDieGameState::IsRunActive() const
{
    return bRunActive;
}

// ---------------- Save/Load ----------------

void ARideOrDieGameState::LoadLeaderboard()
{
    if (USaveGame* Raw = UGameplayStatics::LoadGameFromSlot(SaveSlot, 0))
    {
        if (USG_LeaderboardSave_Cpp* Save = Cast<USG_LeaderboardSave_Cpp>(Raw))
        {
            Leaderboard = Save->Entries;
            SortLeaderboard();
            OnLeaderboardChanged.Broadcast();
            return;
        }
    }

    Leaderboard.Reset();
    OnLeaderboardChanged.Broadcast();
}

void ARideOrDieGameState::SaveLeaderboard()
{
    USG_LeaderboardSave_Cpp* Save =
        Cast<USG_LeaderboardSave_Cpp>(UGameplayStatics::CreateSaveGameObject(USG_LeaderboardSave_Cpp::StaticClass()));

    if (!Save)
        return;

    Save->Entries = Leaderboard;
    UGameplayStatics::SaveGameToSlot(Save, SaveSlot, 0);
}

void ARideOrDieGameState::SortLeaderboard()
{
    // Sort by Highscore desc, then TimeSeconds asc, then Level desc (tiebreak)
    Leaderboard.Sort([](const FLeaderboardRuntimeEntry& A, const FLeaderboardRuntimeEntry& B)
        {
            if (A.Highscore != B.Highscore) return A.Highscore > B.Highscore;
            if (A.TimeSeconds != B.TimeSeconds) return A.TimeSeconds < B.TimeSeconds;
            return A.Level > B.Level;
        });
}

// ---------------- Submission APIs ----------------

void ARideOrDieGameState::SubmitRun(const FString& Name, float TimeSeconds, float Highscore, int32 Level)
{
    FLeaderboardRuntimeEntry E;
    E.Name = Name;
    E.TimeSeconds = TimeSeconds;
    E.Highscore = Highscore;
    E.Level = Level;

    Leaderboard.Add(E);
    SortLeaderboard();

    if (Leaderboard.Num() > MaxEntries)
        Leaderboard.SetNum(MaxEntries);

    SaveLeaderboard();
    OnLeaderboardChanged.Broadcast();
}

void ARideOrDieGameState::SetPendingRunResults(float TimeSeconds, float Highscore, int32 Level)
{
    bHasPendingRun = true;
    PendingTimeSeconds = TimeSeconds;
    PendingHighscore = Highscore;
    PendingLevel = Level;
}

bool ARideOrDieGameState::HasPendingRun() const
{
    return bHasPendingRun;
}

void ARideOrDieGameState::ClearPendingRun()
{
    bHasPendingRun = false;
    PendingTimeSeconds = 0.f;
    PendingHighscore = 0.f;
    PendingLevel = 0;
}

void ARideOrDieGameState::ClearLeaderboard()
{
    Leaderboard.Reset();
    SaveLeaderboard();
    OnLeaderboardChanged.Broadcast();
}

bool ARideOrDieGameState::IsSameEntry(const FLeaderboardRuntimeEntry& A, const FLeaderboardRuntimeEntry& B) const
{
    return A.Name == B.Name
        && FMath::IsNearlyEqual(A.TimeSeconds, B.TimeSeconds, 0.001f)
        && FMath::IsNearlyEqual(A.Highscore, B.Highscore, 0.001f)
        && A.Level == B.Level;
}

void ARideOrDieGameState::SubmitPendingRun(const FString& Name)
{
    if (!bHasPendingRun)
        return;

    // Build the entry we intend to submit
    FLeaderboardRuntimeEntry E;
    E.Name = Name;
    E.TimeSeconds = PendingTimeSeconds;
    E.Highscore = PendingHighscore;
    E.Level = PendingLevel;

    // Double-fire guard (same entry submitted twice quickly)
    const double NowWall = FPlatformTime::Seconds();
    if (bHasLastSubmittedEntry && (NowWall - LastSubmitWallTimeSeconds) < 0.35)
    {
        if (IsSameEntry(E, LastSubmittedEntry))
        {
            ClearPendingRun();
            return;
        }
    }

    LastSubmitWallTimeSeconds = NowWall;
    LastSubmittedEntry = E;
    bHasLastSubmittedEntry = true;

    // Submit
    Leaderboard.Add(E);
    SortLeaderboard();

    if (Leaderboard.Num() > MaxEntries)
        Leaderboard.SetNum(MaxEntries);

    SaveLeaderboard();
    OnLeaderboardChanged.Broadcast();

    // Consume pending
    ClearPendingRun();
}
