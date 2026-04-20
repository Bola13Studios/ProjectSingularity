#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Character/Enemy/BaseEnemy.h"
#include "Gameplay/Weapons/Projectile.h"
#include "StreamerEnemy.generated.h"

UCLASS()
class PROJECTSINGULARITY_API AStreamerEnemy : public ABaseEnemy
{
  GENERATED_BODY()

  virtual void Tick(float DeltaTime) override;

public:
  void SetPath(TArray<FVector> newPath);
  bool HasReachedDestination();
  virtual void Attack(AActor* target) override;
  
  UPROPERTY(EditAnywhere)
  TSubclassOf<AProjectile> projectileClass;

  TArray<FVector> path;
};
