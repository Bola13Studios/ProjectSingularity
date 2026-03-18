#include "Gameplay/Weapons/WeaponBase.h"
#include "GameFramework/Character.h"
#include <Gameplay/Character/Player/PlayerCharacter.h>
#include <Gameplay/Animation/BaseAnimInstance.h>

AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = weaponMesh;

	m_elapsedShootTime = 0.f;
}

const void AWeaponBase::SetWeaponData(FWeaponData weaponData)
{
	m_weaponData = weaponData;
	m_currentWeaponMode = &m_weaponData.firstMode;

	if (weaponMesh && m_weaponData.skeletalMesh)
	{
		weaponMesh->SetSkeletalMesh(m_weaponData.skeletalMesh);
		//weaponMesh->bOwnerNoSee = true;
		//weaponMesh->SetHiddenInGame(true);
		weaponMesh->CastShadow = false;
		weaponMesh->SetAnimInstanceClass(m_weaponData.animInstance);
	}
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* PlayerChar = Cast<ACharacter>(GetOwner());
	if (PlayerChar)
	{
		m_cameraComponent = PlayerChar->FindComponentByClass<UCameraComponent>();;
	}
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	m_elapsedShootTime += DeltaTime;
}

bool AWeaponBase::Fire()
{
	float timeBetweenShots = 60.f / m_currentWeaponMode->fireRateRPM;
	if (!m_cameraComponent || m_elapsedShootTime < timeBetweenShots)
	{
		return true;
	}

	m_elapsedShootTime = 0.f;
	APlayerCharacter* player = Cast<APlayerCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn());
	Cast<UBaseAnimInstance>(player->GetArmsMesh()->GetAnimInstance())->Fire();

	for (int i = 0; i < m_currentWeaponMode->bulletsPerShot; ++i)
	{
		FVector forward = m_cameraComponent->GetForwardVector();

		if (m_currentWeaponMode->bulletSpreadAngle > 0.f)
		{
			float halfAngleRad = FMath::DegreesToRadians(m_currentWeaponMode->bulletSpreadAngle / 2.f);
			forward = FMath::VRandCone(forward, halfAngleRad);
		}

		FVector start = m_cameraComponent->GetComponentLocation();
		FVector end = start + forward * m_currentWeaponMode->bulletDistance;

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
				if (!hit.GetActor())
				{
					continue;
				}

				//Dmg method - TO DO
				DrawDebugSphere(GetWorld(), hit.ImpactPoint, 5.f, 12, FColor::Green, false, 2.f);

				penetrationCount++;

				if (penetrationCount > m_currentWeaponMode->bulletPenetration)
				{
					break;
				}
			}
		}
		else
		{
			DrawDebugSphere(GetWorld(), end, 5.f, 12, FColor::Red, false, 2.f);
		}
	}


	switch (m_currentWeaponMode->fireMode)
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

void AWeaponBase::ChangeWeaponMode() 
{
	UAnimInstance* AnimInstance = weaponMesh->GetAnimInstance();

	if (!AnimInstance) return;

	AnimInstance->Montage_Play(m_weaponData.animMontage);

	if (m_currentWeaponMode != &m_weaponData.firstMode)
	{
		m_currentWeaponMode = &m_weaponData.firstMode;
		AnimInstance->Montage_JumpToSection(FName("Mode2_To_Mode1"), m_weaponData.animMontage);
	}
	else
	{
		m_currentWeaponMode = &m_weaponData.secondMode;
		AnimInstance->Montage_JumpToSection(FName("Mode1_To_Mode2"), m_weaponData.animMontage);
	}
}

