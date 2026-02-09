// UpgradeTypes.h
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/Texture2D.h"
#include "UpgradeTypes.generated.h"

// Keep this tiny + stable. This is what buttons call.
UENUM(BlueprintType)
enum class EUpgradeId : uint8
{
	MaxHealth        UMETA(DisplayName = "Max Health"),
	Damage           UMETA(DisplayName = "Damage"),
	MoveSpeed        UMETA(DisplayName = "Move Speed"),
	ScoreMultiplier  UMETA(DisplayName = "Score Multiplier"),
	BuffDuration     UMETA(DisplayName = "Buff Duration"),
};

// What the UI needs to render a card.
USTRUCT(BlueprintType)
struct FUpgradeDef : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUpgradeId Id = EUpgradeId::MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;

	// Weight for random roll. Higher = more likely.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Weight = 10;

	// If you want upgrades to stack: rank can control magnitude.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float BaseValue = 1.0f;
};
