#include "Gameplay/Weapons/WeaponBase.h"
#include "GameFramework/Character.h"
#include <Gameplay/Character/Player/PlayerCharacter.h>
#include <Gameplay/Animation/BaseAnimInstance.h>
#include "Components/ActionStateFilter.h"
#include "Utils/State Machine/States.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"

AWeaponBase::AWeaponBase()
{
  PrimaryActorTick.bCanEverTick = true;

  weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
  RootComponent = weaponMesh;

  m_actionsFilterComponent = CreateDefaultSubobject<UActionStateFilter>(TEXT("ActionsFilter"));
}

void AWeaponBase::BeginPlay()
{
  Super::BeginPlay();
}

void AWeaponBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  m_timeSinceLastShot += DeltaTime;

#if WITH_EDITOR
  if (m_player->GetDebugWeapon() && m_actionsFilterComponent->StatesDataAsset != NULL)
  {
    UWorld* world = GetWorld();
    FVector actorLocation = GetActorUpVector() * 30 + GetActorForwardVector() * 30;
    FString text = m_actionsFilterComponent->GetCurrentBaseState()->GetFName().ToString();
    DrawDebugString(world, actorLocation, text, this, FColor::Red, 0.0f, false, 2);
  }
#endif
}

const void AWeaponBase::SetWeaponData(FWeaponData weaponData)
{
  m_weaponData = weaponData;

  // Initialize filter
  m_actionsFilterComponent->InitializeFilter(this, m_weaponData.statesDataAsset, UWeaponIdle::StaticClass());

  // Initialize modes
  m_firstMode.Initialize(m_weaponData.firstMode);
  m_secondMode.Initialize(m_weaponData.secondMode);
  m_currentWeaponMode = &m_firstMode;
  m_currentAmmoInReser = m_weaponData.maxAmmoInReser;

  // Set weapon mesh and anim instance
  if (weaponMesh && m_weaponData.skeletalMesh)
  {
    weaponMesh->SetSkeletalMesh(m_weaponData.skeletalMesh);
    weaponMesh->SetAnimInstanceClass(m_weaponData.weaponAnimInstance);
    weaponMesh->SetCastShadow(false);
  }

  // Get references
  m_player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
  m_weaponAnimInstance = weaponMesh->GetAnimInstance();
  m_armsAnimInstance = m_player->GetArmsMesh()->GetAnimInstance();
  m_cameraComponent = m_player->FindComponentByClass<UCameraComponent>();
  ;
}

bool AWeaponBase::Fire()
{
  // Seconds in a minute divided by the fire rate (Rounds Per Minute)
  float timeBetweenShots = 60.f / m_currentWeaponMode->GetModeData().fireRateRPM;

  if (m_actionsFilterComponent->GetCurrentBaseStateClass() != UWeaponIdle::StaticClass()
      || m_timeSinceLastShot < timeBetweenShots)
  {
    return true;
  }

  if (m_currentWeaponMode->currentAmmoInMag <= 0)
  {
    TryToReload();
    return true;
  }

  // Play animation
  if (m_currentWeaponMode == &m_firstMode)
  {
    PlayAnimation(FName("Mode1_Shot"));
  }
  else
  {
    PlayAnimation(FName("Mode2_Shot"));
  }

  m_timeSinceLastShot = 0.f;
  m_currentWeaponMode->currentAmmoInMag--;

  for (int i = 0; i < m_currentWeaponMode->GetModeData().bulletsPerShot; ++i)
  {
    FVector forward = m_cameraComponent->GetForwardVector();

    if (m_currentWeaponMode->GetModeData().bulletSpreadAngle > 0.f)
    {
      float halfAngleRad = FMath::DegreesToRadians(m_currentWeaponMode->GetModeData().bulletSpreadAngle / 2.f);
      forward = FMath::VRandCone(forward, halfAngleRad);
    }

    FVector start = m_cameraComponent->GetComponentLocation();
    FVector end = start + forward * m_currentWeaponMode->GetModeData().bulletDistance;

    TArray<FHitResult> hits;
    FCollisionQueryParams params;
    params.AddIgnoredActor(this);
    params.AddIgnoredActor(GetOwner());

    bool bHit = GetWorld()->LineTraceMultiByChannel(hits, start, end, ECC_Visibility, params);
    int penetrationCount = 0;

    if (hits.Num() > 0)
    {
      for (const FHitResult& hit : hits)
      {
        AActor* hitActor = hit.GetActor();
        if (!hitActor)
        {
          continue;
        }

        // Dmg method - TO DO
        //m_currentWeaponMode->bulletDmg + m_currentWeaponMode->extraBulletDmg

        if (m_player->GetDebugWeapon())
        {
          DrawDebugSphere(GetWorld(), hit.ImpactPoint, 5.f, 12, FColor::Green, false, 2.f);
        }

        if (hitActor)
        {
          UHealthComponent* healthComp = hitActor->FindComponentByClass<UHealthComponent>();

          // changing health
          if (healthComp)
          {
            healthComp->ChangeHealth(-m_currentWeaponMode->GetModeData().bulletDamage, GetOwner());
          }
        }

        penetrationCount++;
        if (penetrationCount > m_currentWeaponMode->GetModeData().bulletPenetration)
        {
          break;
        }
      }
    }
    else
    {
      if (m_player->GetDebugWeapon())
      {
        DrawDebugSphere(GetWorld(), end, 5.f, 12, FColor::Red, false, 2.f);
      }
    }
  }

  // To do
  switch (m_currentWeaponMode->GetModeData().fireMode)
  {
    case EFireMode::None:
      return false;
      break;
    case EFireMode::SemiAuto:
      return false;
      break;
    case EFireMode::FullAuto:
      return true;
      break;
    case EFireMode::Burst:
      break;
    default:
      break;
  }

  return true;
}

#pragma region ReloadFunctions
void AWeaponBase::TryToReload()
{
  if (!m_currentWeaponMode) return;

  if (m_currentWeaponMode->currentAmmoInMag >= m_currentWeaponMode->GetModeData().maxAmmoInMag) return;

  if (m_currentAmmoInReser <= 0) return;

  m_actionsFilterComponent->SetCurrentState(UWeaponReload::StaticClass());
}

void AWeaponBase::Reload()
{
  if (m_currentWeaponMode == &m_firstMode)
  {
    PlayAnimation(FName("Mode1_Reload"));
  }
  else
  {
    PlayAnimation(FName("Mode2_Reload"));
  }

  int maxMag = m_currentWeaponMode->GetModeData().maxAmmoInMag;

  int missingAmmo = maxMag - m_currentWeaponMode->currentAmmoInMag;

  int ammoToReload = FMath::Min(missingAmmo, m_currentAmmoInReser);

  m_currentWeaponMode->currentAmmoInMag += ammoToReload;
  m_currentAmmoInReser -= ammoToReload;
}

// Called by an event in the weapon's anim instance
void AWeaponBase::OnReloadFinished()
{
  m_actionsFilterComponent->SetCurrentState(UWeaponIdle::StaticClass());
}
#pragma endregion

#pragma region ChangeModeFunctions
void AWeaponBase::TryToChangeMode()
{
  m_actionsFilterComponent->SetCurrentState(UWeaponChangeMode::StaticClass());
}

void AWeaponBase::ChangeMode()
{
  if (m_currentWeaponMode != &m_firstMode)
  {
    m_currentWeaponMode = &m_firstMode;
    PlayAnimation(FName("Mode2_To_Mode1"));
  }
  else
  {
    m_currentWeaponMode = &m_secondMode;
    PlayAnimation(FName("Mode1_To_Mode2"));
  }
}

// Called by an event in the weapon's anim instance
void AWeaponBase::OnChangeModeFinished()
{
  m_actionsFilterComponent->SetCurrentState(UWeaponIdle::StaticClass());
}
#pragma endregion

void AWeaponBase::PlayAnimation(FName name)
{
  m_weaponAnimInstance->Montage_Play(m_weaponData.weaponAnimMontage);
  m_weaponAnimInstance->Montage_JumpToSection(name, m_weaponData.weaponAnimMontage);

  m_armsAnimInstance->Montage_Play(m_weaponData.armsAnimMontage);
  m_armsAnimInstance->Montage_JumpToSection(name, m_weaponData.armsAnimMontage);
}

int AWeaponBase::GetAmmoInMagazine()
{
  return m_currentWeaponMode->currentAmmoInMag;
}

int AWeaponBase::GetAmmoInReserve()
{
  return m_currentAmmoInReser;
}

void AWeaponBase::AddReserveAmmo(int extraAmmo)
{
  m_currentAmmoInReser += extraAmmo;
}

void AWeaponBase::AddExtraBulletDmg(int extraBulletDmg, bool firstMode)
{
  if (firstMode)
  {
    m_firstMode.extraBulletDmg += extraBulletDmg;
  }
  else
  {
    m_secondMode.extraBulletDmg += extraBulletDmg;
  }
}

void AWeaponBase::SetExtraBulletDmg(int extraBulletDmg, bool firstMode)
{
  if (firstMode)
  {
    m_firstMode.extraBulletDmg = extraBulletDmg;
  }
  else
  {
    m_secondMode.extraBulletDmg = extraBulletDmg;
  }
}

int AWeaponBase::GetExtraBulletDmg(bool firstMode)
{
  if (firstMode)
  {
    return m_firstMode.extraBulletDmg;
  }
  else
  {
    return m_secondMode.extraBulletDmg;
  }
}
