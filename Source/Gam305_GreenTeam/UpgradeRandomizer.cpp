// UpgradeRandomizer.cpp
#include "UpgradeRandomizer.h"

static int32 WeightedPickIndex(const TArray<FUpgradeDef>& Items, const TArray<int32>& Indices, FRandomStream& Rng)
{
	int32 TotalWeight = 0;
	for (int32 Idx : Indices)
	{
		TotalWeight += FMath::Max(0, Items[Idx].Weight);
	}

	if (TotalWeight <= 0)
	{
		return Indices.Num() > 0 ? Indices[Rng.RandRange(0, Indices.Num() - 1)] : INDEX_NONE;
	}

	int32 Roll = Rng.RandRange(1, TotalWeight);
	for (int32 Idx : Indices)
	{
		Roll -= FMath::Max(0, Items[Idx].Weight);
		if (Roll <= 0) return Idx;
	}
	return Indices.Last();
}

TArray<FUpgradeDef> UUpgradeRandomizer::PickRandomUpgrades(
	const TArray<FUpgradeDef>& Pool,
	int32 Count,
	const TArray<EUpgradeId>& ExcludeIds,
	int32 Seed
)
{
	TSet<EUpgradeId> ExcludeSet;
	for (EUpgradeId Id : ExcludeIds) ExcludeSet.Add(Id);

	TArray<int32> Candidates;
	Candidates.Reserve(Pool.Num());

	for (int32 i = 0; i < Pool.Num(); i++)
	{
		if (!ExcludeSet.Contains(Pool[i].Id))
		{
			Candidates.Add(i);
		}
	}

	Count = FMath::Clamp(Count, 0, Candidates.Num());

	FRandomStream Rng(Seed != 0 ? Seed : (int32)FPlatformTime::Cycles());

	TArray<FUpgradeDef> Out;
	Out.Reserve(Count);

	// Unique picks: remove the chosen index each time
	for (int32 Pick = 0; Pick < Count; Pick++)
	{
		const int32 PickedPoolIndex = WeightedPickIndex(Pool, Candidates, Rng);
		if (PickedPoolIndex == INDEX_NONE) break;

		Out.Add(Pool[PickedPoolIndex]);
		Candidates.RemoveSingleSwap(PickedPoolIndex);
	}

	return Out;
}
