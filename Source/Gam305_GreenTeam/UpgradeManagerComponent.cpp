#include "UpgradeManagerComponent.h"

bool UUpgradeManagerComponent::HasUpgrade(EUpgradeId Id) const
{
	return OwnedUpgrades.Contains(Id);
}

void UUpgradeManagerComponent::ApplyUpgrade(const FUpgradeDef& UpgradeDef)
{
	const EUpgradeId Id = UpgradeDef.Id;
	const float Value = UpgradeDef.BaseValue;

	// Prevent duplicates (remove if you want stacking)
	if (OwnedUpgrades.Contains(Id))
		return;

	switch (Id)
	{
	case EUpgradeId::MaxHealth:
		MaxHealth += Value;
		break;

	case EUpgradeId::Damage:
		Damage += Value;
		break;

	case EUpgradeId::MoveSpeed:
		MoveSpeed += Value;
		break;

	case EUpgradeId::ScoreMultiplier:
		ScoreMultiplier += Value;
		break;

	case EUpgradeId::BuffDuration:
		BuffDurationMultiplier += Value;
		break;

	default:
		break;
	}

	OwnedUpgrades.Add(Id);

	// Broadcast the full def so listeners can Break it and read BaseValue/Name/etc
	OnUpgradeApplied.Broadcast(UpgradeDef);
}
