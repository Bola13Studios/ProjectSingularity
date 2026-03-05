/************************************************************************
 * @description: Base class for character
 * @author: Rub�n Santos
 * @date: 01/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Weapons/WeaponBase.h"
#include "Weapons/WeaponsDataAsset.h"
#include <EnhancedActionKeyMapping.h>

#include "Components/HealthComponent.h"
#include "PACharacter.generated.h"

UCLASS()                  // Primetime Asset Character - Design "name"
class PROJECTSINGULARITY_API APACharacter : public ACharacter 
{
    GENERATED_BODY()

public:
    // Sets default values for this character's properties
    APACharacter();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    virtual void PossessedBy(AController* NewController) override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UPROPERTY(EditAnywhere, Category = "Input")
    TSoftObjectPtr<UInputMappingContext> inputMapping;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputAction* IA_Fire;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    TSubclassOf<AWeaponBase> weaponClass;

    UPROPERTY(EditDefaultsOnly, Category = "Weapon")
    UWeaponsDataAsset* weaponDataAsset;


private:
    UFUNCTION()
    void StartFire(const FInputActionValue& Value);
    UFUNCTION()
    void StopFire();

    // Get the Health component referenced in blueprint here

    UFUNCTION(BlueprintCallable)
    UHealthComponent* GetHealthComponent() const;

    UPROPERTY()
    AWeaponBase* m_currentWeapon;

    bool m_bFire = false;
};
