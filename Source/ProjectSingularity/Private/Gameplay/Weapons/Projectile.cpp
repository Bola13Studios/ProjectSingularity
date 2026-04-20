#include "Gameplay/Weapons/Projectile.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AProjectile::AProjectile()
{
  PrimaryActorTick.bCanEverTick = false;

  collider = CreateDefaultSubobject<USphereComponent>("Collision");
  RootComponent = collider;

  collider->InitSphereRadius(20.f);
  collider->SetCollisionProfileName("BlockAllDynamic");
  collider->SetNotifyRigidBodyCollision(true);
  collider->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

  mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
  mesh->SetupAttachment(RootComponent);
  mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  movementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");
  movementComponent->InitialSpeed = initialSpeed;
  movementComponent->MaxSpeed = maxSpeed;
  movementComponent->bRotationFollowsVelocity = true;
  movementComponent->bShouldBounce = false;
}

void AProjectile::BeginPlay()
{
  Super::BeginPlay();

  if (AActor* owner = GetOwner())
  {
    collider->IgnoreActorWhenMoving(owner, true);
  }
}

void AProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
  if (OtherActor == GetOwner()) return;

  UWorld* World = GetWorld();
  if (!World) return;

  //Debug
  DrawDebugSphere(World, Hit.ImpactPoint, explosionRadius, 20, FColor::Red, false, 2.f);

  // Implement dmg
  Destroy();
}

float AProjectile::GetInitialSpeed() const
{
  return initialSpeed;
}
UProjectileMovementComponent* AProjectile::GetMovementComponent()
{
  return movementComponent;
}
void AProjectile::FireInDirection(const FVector& Direction)
{
  movementComponent->Velocity = Direction * movementComponent->InitialSpeed;
}

void AProjectile::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (m_timeDestroyProjectile <= m_elapsedTimeDestroyProjectile)
  {
    Destroy();
  }
  m_elapsedTimeDestroyProjectile += DeltaTime;
}
