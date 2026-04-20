#include "Gameplay/Character/Enemy/ChildEnemies/StreamerEnemy.h"
#include "Gameplay/Weapons/Projectile.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

void AStreamerEnemy::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (path.Num() == 0)
  {
    return;
  }

  FVector currentPos = GetActorLocation();
  FVector target = path[0];

  float distance = FVector::Dist(currentPos, target);
  

  if (path.Num() > 1)
  {
    float distance2 = FVector::Dist(currentPos, path[1]);
    if (distance2 < distance)
    {
      target = path[1];
      path.RemoveAt(0);
    }
  }

  FVector dir = (target - currentPos).GetSafeNormal();

  float speedFactor = FMath::Clamp(distance / 300.f, 0.2f, 1.f);

  AddMovementInput(dir, speedFactor);

  float acceptanceRadius = 300.f;

  //debug
  for (int i = 0; i < path.Num() - 1; i++)
  {
    DrawDebugLine(GetWorld(), path[i], path[i + 1], FColor::Cyan, false, 10.f, 0, 0);
  }

  if (distance < acceptanceRadius)
  {
    path.RemoveAt(0);
  }
}

void AStreamerEnemy::SetPath(TArray<FVector> newPath)
{
  path = newPath;
}

bool AStreamerEnemy::HasReachedDestination()
{
  return path.Num() == 0;
}

void AStreamerEnemy::Attack(AActor* target)
{
  Super::Attack(target);

  UWorld* world = GetWorld();
  if (!target || !world || !projectileClass)
  {
    return;
  }

  FVector streamerLocation = GetActorLocation();
  FVector direction = (target->GetActorLocation() - streamerLocation).GetSafeNormal();

  FRotator rotation = direction.Rotation();

  FActorSpawnParameters params;
  params.Owner = this;
  params.Instigator = GetInstigator();
  params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

  AProjectile* projectile = world->SpawnActor<AProjectile>(projectileClass, streamerLocation, rotation, params);

  if (!projectile)
  {
    return;
  }

  //projectile->FireInDirection(direction);

  if (UPrimitiveComponent* comp = Cast<UPrimitiveComponent>(projectile->GetRootComponent()))
  {
    comp->IgnoreActorWhenMoving(this, true);
    if (UPrimitiveComponent* streamer = Cast<UPrimitiveComponent>(this))
    {
      streamer->IgnoreActorWhenMoving(projectile, true);
    }
  }
}
