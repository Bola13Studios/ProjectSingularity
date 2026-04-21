#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UProjectileMovementComponent;

UCLASS()
class PROJECTSINGULARITY_API AProjectile : public AActor
{
  GENERATED_BODY()

public:
  AProjectile();

protected:
  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  UFUNCTION()
  void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse,
             const FHitResult& Hit);

public:
  float GetInitialSpeed() const;
  UProjectileMovementComponent* GetMovementComponent();
  void FireInDirection(const FVector& Direction);

protected:
  UPROPERTY(VisibleAnywhere)
  USphereComponent* collider;

  UPROPERTY(VisibleAnywhere)
  USkeletalMeshComponent* mesh;

  UPROPERTY(VisibleAnywhere)
  UProjectileMovementComponent* movementComponent;

public:
  UPROPERTY(EditAnywhere)
  float maxSpeed = 2000.f;

  UPROPERTY(EditAnywhere)
  float initialSpeed = 2000.f;

  UPROPERTY(EditAnywhere)
  float explosionRadius = 200.f;

private:
  float m_timeDestroyProjectile = 15.f;
  float m_elapsedTimeDestroyProjectile = 0.f;
};
