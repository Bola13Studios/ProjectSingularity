/************************************************************************
 * @description: Data asset for weapons
 * @author: Rubén Santos
 * @date: 01/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponBase.h"
#include "Utils/State Machine/States.h"
#include "WeaponsDataAsset.generated.h"



UCLASS(BlueprintType)
class PROJECTSINGULARITY_API UWeaponsDataAsset : public UDataAsset
{
    GENERATED_BODY()
public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TArray<FWeaponData> weaponsData;

#if WITH_EDITOR
    virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
