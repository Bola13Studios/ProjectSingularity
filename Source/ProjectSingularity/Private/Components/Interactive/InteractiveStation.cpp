#include "Components/Interactive/InteractiveStation.h"
#include "Engine/World.h"
#include "Components/Interactive/StationData.h"
#include "Components/Interactive/StationStates.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Gameplay/Character/Player/PlayerCharacter.h"
#include "ProjectSingularity/Public/Gameplay/Weapons/WeaponBase.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "ProjectSingularity/Public/Utils/StatHelpers.h"

void UInteractiveStation::Interact()
{
  float totalSpent = 0.0f;
  // we check if the asset provided is valid
  TArray<FStationData*> StationData;
  if (!IsValid(m_StationData))
  {
    UE_LOG(LogTemp, Warning, TEXT("Invalid Station Data Table added in InteractiveStation BP. Unable to procede."));
    return;
  }
  // we check if the asset has elements
  m_StationData->GetAllRows(TEXT("Station"), StationData);
  if (!StationData.IsValidIndex(0))
  {
    UE_LOG(LogTemp, Warning,
           TEXT("The provided Station Data Table is empty or an error occurred while trying to access it."));
    return;
  }

  for (auto Row : StationData)
  {
    if (m_stationMode == Row->m_Type)
    {
      totalSpent += Row->m_TypePrice;

      switch (m_stationMode)
      {
        case EStationStates::HEALTH:
          if (!ChangeHealth(Row->m_TypeAmount, Row->m_TypePrice))
          {
            UE_LOG(LogTemp, Warning, TEXT("Something went wrong? Unable to change health value."));
          }
          break;

        case EStationStates::AMMO:
          if (!ChangeAmmo(Row->m_TypeAmount, Row->m_TypePrice))
          {
            UE_LOG(LogTemp, Warning, TEXT("Something went wrong? Unable to change ammo value."));
          }
          break;

        default:
          UE_LOG(LogTemp, Warning,
                 TEXT("Station State not recognized or available. Remember to add it to the InteractiveStation"));
          return;
          break;
      }
    }
  }

  OnInteract.Broadcast();
}

void UInteractiveStation::OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
  // we check if the ovelap was with the player
  if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
  {
    m_Player = Player;

    // we bind the method to the delegate
    m_Player->m_onInteract.AddUObject(this, &UBaseInteractiveComponent::Interact);
  }
}

void UInteractiveStation::OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // we check if the overlap was with the player
  if (Cast<AActor>(m_Player) == OtherActor)
  {
    // we unbind the method to the delegate
    m_Player->m_onInteract.RemoveAll(this);

    m_Player = nullptr;
  }
}

bool UInteractiveStation::ChangeHealth(const float& _Amount, const float& _Cost)
{
  // we check if player object is valid
  if (!IsValid(m_Player)) return false;

  // --- health component --- //
  UHealthComponent* HealthComponent = m_Player->FindComponentByClass<UHealthComponent>();
  // --- hype component   --- //
  UHypeReceiverComponent* HypeComponent = m_Player->FindComponentByClass<UHypeReceiverComponent>();

  // we verify if both the components are present on the player
  if (!IsValid(HealthComponent) || !IsValid(HypeComponent))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return false;
  }

  // we check if the player has enough hype
  if (!HypeComponent->IsHypeEnough(_Cost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return false;
  }

  HypeComponent->AddHype(-_Cost);
  HealthComponent->ChangeHealth(_Amount, GetOwner());
  UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_used_hype_onhealth), _Cost);

  return true;
}

bool UInteractiveStation::ChangeAmmo(const float& _Amount, const float& _Cost)
{
  // we check if player object is valid
  if (!IsValid(m_Player)) return false;

  // --- ammo?            --- //
  // --- hype component   --- //
  UHypeReceiverComponent* HypeComponent = m_Player->FindComponentByClass<UHypeReceiverComponent>();
  AWeaponBase* CurrentWeapon = m_Player->GetWeapon();

  // we verify if both the components are present on the player
  if (!IsValid(HypeComponent) || !IsValid(CurrentWeapon))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return false;
  }

  // we check if the player has enough hype
  if (!HypeComponent->IsHypeEnough(_Cost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return false;
  }

  HypeComponent->AddHype(-_Cost);
  CurrentWeapon->AddReserveAmmo(_Amount);

  UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_used_hype_onammo), _Cost);

  return true;
}
