#include "Gameplay/Weapons/WeaponBase.h"
#include "GameFramework/Character.h"

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
	if (weaponMesh && m_weaponData.skeletalMesh)
	{
		weaponMesh->SetSkeletalMesh(m_weaponData.skeletalMesh);
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
	float timeBetweenShots = 60.f / m_weaponData.fireRateRPM;
	if (!m_cameraComponent || m_elapsedShootTime < timeBetweenShots)
	{
		return true;
	}

	m_elapsedShootTime = 0.f;

	for (int i = 0; i < m_weaponData.bulletsPerShot; ++i)
	{
		FVector forward = m_cameraComponent->GetForwardVector();

		if (m_weaponData.bulletSpreadAngle > 0.f)
		{
			float halfAngleRad = FMath::DegreesToRadians(m_weaponData.bulletSpreadAngle / 2.f);
			forward = FMath::VRandCone(forward, halfAngleRad);
		}

		FVector start = m_cameraComponent->GetComponentLocation();
		FVector end = start + forward * m_weaponData.bulletDistance;

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

				if (penetrationCount > m_weaponData.bulletPenetration)
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


	switch (m_weaponData.fireMode)
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

