#include "WeaponAnimInstance.h"
#include "Gameplay/Weapons/WeaponBase.h"


void UWeaponAnimInstance::AnimNotify_ReloadFinished()
{
    AWeaponBase* Weapon = Cast<AWeaponBase>(GetOwningActor());

    if (Weapon)
    {
        Weapon->OnReloadFinished();
    }
}

void UWeaponAnimInstance::AnimNotify_ChangeModeFinished()
{
    AWeaponBase* Weapon = Cast<AWeaponBase>(GetOwningActor());

    if (Weapon)
    {
        Weapon->OnChangeModeFinished();
    }
}