/************************************************************************
 * @description: Anim Instance class for weapons
 * @author: Rubén Santos
 * @date: 22/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Animation/BaseAnimInstance.h"
#include "WeaponAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTSINGULARITY_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	UFUNCTION()
	void AnimNotify_ReloadFinished();

	UFUNCTION()
	void AnimNotify_ChangeModeFinished();
};
