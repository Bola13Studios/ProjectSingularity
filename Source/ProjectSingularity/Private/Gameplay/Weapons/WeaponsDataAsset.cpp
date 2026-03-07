#include "Gameplay/Weapons/WeaponsDataAsset.h"

#if WITH_EDITOR
void UWeaponsDataAsset::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);

	for (int32 i = 0; i < weaponsData.Num(); ++i)
	{
		weaponsData[i].ID = i;
	}
}
#endif
