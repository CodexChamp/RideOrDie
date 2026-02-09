// RideOrDieGameState.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "LeaderboardRuntimeTypes.h"
#include "RideOrDieGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLeaderboardChanged);

UCLASS()
class GAM305_GREENTEAM_API ARideOrDieGameState : public AGameStateBase
{
    GENERATED_BODY()

public:
    ARideOrDieGameState();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;

    // ---------------- Leaderboard ----------------
    UPROPERTY(BlueprintReadOnly)
    TArray<FLeaderboardRuntimeEntry> Leaderboard;

    UPROPERTY(BlueprintAssignable)
    FOnLeaderboardChanged OnLeaderboardChanged;

    // --- Old API (still supported if you want to call it directly) ---
    UFUNCTION(BlueprintCallable)
    void SubmitRun(const FString& Name, float TimeSeconds, float Highscore, int32 Level);

    UFUNCTION(BlueprintCallable)
    const TArray<FLeaderboardRuntimeEntry>& GetLeaderboard() const;

    // ---------------- Run Timer (GameState owns it) ----------------
    // Call when the run should start (BeginPlay of pawn, or when you unpause into gameplay)
    UFUNCTION(BlueprintCallable)
    void StartRunTimer();

    // Call when the run ends (death)
    UFUNCTION(BlueprintCallable)
    void StopRunTimer();

    // Optional: reset start time and current time to 0
    UFUNCTION(BlueprintCallable)
    void ResetRunTimer();

    // Read current run time (seconds)
    UFUNCTION(BlueprintCallable)
    float GetCurrentRunTime() const;

    UFUNCTION(BlueprintCallable)
    bool IsRunActive() const;

    // --- New "Option A" API ---
    // Call this ONCE when the run ends (death).
    UFUNCTION(BlueprintCallable)
    void SetPendingRunResults(float TimeSeconds, float Highscore, int32 Level);

    // Call this from WBP_Died after the player types a name.
    UFUNCTION(BlueprintCallable)
    void SubmitPendingRun(const FString& Name);

    // Optional helpers for debugging/testing
    UFUNCTION(BlueprintCallable)
    bool HasPendingRun() const;

    UFUNCTION(BlueprintCallable)
    void ClearPendingRun();

    UFUNCTION(BlueprintCallable)
    void ClearLeaderboard();

private:
    void LoadLeaderboard();
    void SaveLeaderboard();
    void SortLeaderboard();

    static constexpr int32 MaxEntries = 25;
    static const FString SaveSlot;

    // ---------------- Timer state ----------------
    UPROPERTY()
    bool bRunActive = false;

    UPROPERTY()
    float RunStartTimeSeconds = 0.f;

    UPROPERTY()
    float CurrentRunTimeSeconds = 0.f;

    // ---------------- Pending run buffer (Option A) ----------------
    bool bHasPendingRun = false;
    float PendingTimeSeconds = 0.f;
    float PendingHighscore = 0.f;
    int32 PendingLevel = 0;

    // Double-fire guard (only blocks same entry fired twice quickly)
    double LastSubmitWallTimeSeconds = -1.0;
    FLeaderboardRuntimeEntry LastSubmittedEntry;
    bool bHasLastSubmittedEntry = false;

    bool IsSameEntry(const FLeaderboardRuntimeEntry& A, const FLeaderboardRuntimeEntry& B) const;
};
