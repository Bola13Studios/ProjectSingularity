#include "Gameplay/Character/PACharacter.h"
#include <EnhancedInputSubsystems.h>
#include "InputMappingContext.h"

#include "EnhancedInputComponent.h"
#include "InputAction.h"

APACharacter::APACharacter()
{
    PrimaryActorTick.bCanEverTick = true;
}

void APACharacter::BeginPlay()
{
    Super::BeginPlay();

    FActorSpawnParameters spawnParams;
    spawnParams.Owner = this;

    m_currentWeapon = GetWorld()->SpawnActor<AWeaponBase>(weaponClass, spawnParams);

    if (m_currentWeapon)
    {
        m_currentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("HandGrip_R")); //Temp bone name
        m_currentWeapon->SetWeaponData(weaponDataAsset->weaponsData[0]); //Just for now
    }

}

void APACharacter::PossessedBy(AController* NewController)
{
    Super::PossessedBy(NewController);

    if (APlayerController* PC = Cast<APlayerController>(NewController))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(inputMapping.LoadSynchronous(), 0);
            }
        }
    }
}

void APACharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (m_bFire && m_currentWeapon)
    {
        m_bFire = m_currentWeapon->Fire();
    }
}

void APACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &APACharacter::StartFire);
        EnhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Completed, this, &APACharacter::StopFire);
    }
}

void APACharacter::StartFire(const FInputActionValue& Value)
{
    m_bFire = true;
}

void APACharacter::StopFire()
{
    m_bFire = false;
}

UHealthComponent* APACharacter::GetHealthComponent() const
{
    // Finds the health component in the blueprint if added
    return FindComponentByClass<UHealthComponent>();
}
