#include "ProjectSingularity/Public/Components/Interactive/InteractiveStation.h"
#include "ProjectSingularity/Public/Components/Interactive/StationStates.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "ProjectSingularity/Public/Gameplay/Character/Player/PlayerCharacter.h"
#include "ProjectSingularity/Public/Gameplay/Weapons/WeaponBase.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "ProjectSingularity/Public/Utils/StatHelpers.h"
#include <Engine/World.h>

void UInteractiveStation::Interact()
{
  switch (m_stationMode)
  {
    case EStationStates::HEALTH:
      if (!ChangeHealth(amount, hypeCost))
      {
        UE_LOG(LogTemp, Warning, TEXT("Something went wrong? Unable to change health value."));
      }
      break;

    case EStationStates::AMMO:
      if (!ChangeAmmo(amount, hypeCost))
      {
        UE_LOG(LogTemp, Warning, TEXT("Something went wrong? Unable to change ammo value."));
      }
      break;

    case EStationStates::MAX_HEALTH:
      if (!ChangeMaxHealth(amount, hypeCost))
      {
        UE_LOG(LogTemp, Warning, TEXT("Something went wrong? Unable to change max health value."));
      }
      break;

    case EStationStates::DAMAGE:
      if (!ChangeDamage(amount, hypeCost))
      {
        UE_LOG(LogTemp, Warning, TEXT("Something went wrong? Unable to change damage value."));
      }
      break;

    default:
      UE_LOG(LogTemp, Warning,
              TEXT("Station State not recognized or available. Remember to add it to the InteractiveStation"));
      return;
      break;
  }

  OnInteract.Broadcast();
}

void UInteractiveStation::OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                 const FHitResult& SweepResult)
{
  // we check if the ovelap was with the player
  if (APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor))
  {
    m_player = player;

    // we bind the method to the delegate
    m_player->m_onInteract.AddUObject(this, &UBaseInteractiveComponent::Interact);
  }
}

void UInteractiveStation::OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // we check if the overlap was with the player
  if (Cast<AActor>(m_player) == OtherActor)
  {
    // we unbind the method to the delegate
    m_player->m_onInteract.RemoveAll(this);

    m_player = nullptr;
  }
}

bool UInteractiveStation::ChangeHealth(const float& _amount, const float& _cost)
{
  // we check if player object is valid
  if (!IsValid(m_player)) return false;

  // --- health component --- //
  UHealthComponent* healthComponent = m_player->FindComponentByClass<UHealthComponent>();
  // --- hype component   --- //
  UHypeReceiverComponent* hypeComponent = m_player->FindComponentByClass<UHypeReceiverComponent>();

  // we verify if both the components are present on the player
  if (!IsValid(healthComponent) || !IsValid(hypeComponent))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return false;
  }

  // we check if the player is trying to heal more than his max health
  if (healthComponent->GetHealth() + _amount > healthComponent->GetMaxHealth())
  {
    UE_LOG(LogTemp, Warning, TEXT("Trying to heal more than max health. Unable to proceed."));
    return false;
  }

  // we check if the player has enough hype
  if (!hypeComponent->IsHypeEnough(_cost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return false;
  }

  hypeComponent->AddHype(-_cost);
  healthComponent->ChangeHealth(_amount, GetOwner());
  UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_used_hype_onhealth), _cost);

  return true;
}

bool UInteractiveStation::ChangeAmmo(const float& _amount, const float& _cost)
{
  // we check if player object is valid
  if (!IsValid(m_player)) return false;

  // --- ammo             --- //
  // --- hype component   --- //
  UHypeReceiverComponent* hypeComponent = m_player->FindComponentByClass<UHypeReceiverComponent>();
  AWeaponBase* currentWeapon = m_player->GetWeapon();

  // we verify if both the components are present on the player
  if (!IsValid(hypeComponent) || !IsValid(currentWeapon))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return false;
  }

  // we check if the player has enough hype
  if (!hypeComponent->IsHypeEnough(_cost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return false;
  }

  hypeComponent->AddHype(-_cost);
  currentWeapon->AddReserveAmmo(_amount);

  UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_used_hype_onammo), _cost);

  return true;
}

bool UInteractiveStation::ChangeMaxHealth(const float& _amount, const float& _cost)
{
  // we check if player object is valid
  if (!IsValid(m_player)) return false;

  // --- health component --- //
  UHealthComponent* healthComponent = m_player->FindComponentByClass<UHealthComponent>();
  // --- hype component   --- //
  UHypeReceiverComponent* hypeComponent = m_player->FindComponentByClass<UHypeReceiverComponent>();

  // we verify if both the components are present on the player
  if (!IsValid(healthComponent) || !IsValid(hypeComponent))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return false;
  }

  // we check if the player has enough hype
  if (!hypeComponent->IsHypeEnough(_cost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return false;
  }

  hypeComponent->AddHype(-_cost);
  healthComponent->SetMaxHealth(healthComponent->GetMaxHealth() + _amount);

  UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_used_hype_onhealth), _cost);
  UGameManagerSubsystem::AddMapStat(this, STAT_MAP(combat.upgrades.combat_end_upgrades), EStationStates::MAX_HEALTH);

  return true;
}

bool UInteractiveStation::ChangeDamage(const float& _amount, const float& _cost)
{
  // we check if player object is valid
  if (!IsValid(m_player)) return false;

  // --- damage           --- //
  // --- hype component   --- //
  UHypeReceiverComponent* hypeComponent = m_player->FindComponentByClass<UHypeReceiverComponent>();
  AWeaponBase* currentWeapon = m_player->GetWeapon();

  // we verify if both the components are present on the player
  if (!IsValid(hypeComponent) || !IsValid(currentWeapon))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return false;
  }

  // we check if the player has enough hype
  if (!hypeComponent->IsHypeEnough(_cost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return false;
  }

  hypeComponent->AddHype(-_cost);
  // we add the damage to both modes
  currentWeapon->AddExtraBulletDmg(_amount, true);
  currentWeapon->AddExtraBulletDmg(_amount, false);

  UGameManagerSubsystem::AddStat(this, STAT_PATH(hype.total_used_hype_ondamage), _cost);
  UGameManagerSubsystem::AddMapStat(this, STAT_MAP(combat.upgrades.combat_end_upgrades), EStationStates::DAMAGE);

  return true;
}
