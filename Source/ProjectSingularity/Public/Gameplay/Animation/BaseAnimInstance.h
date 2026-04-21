/************************************************************************
 * @description:
 * @author: Rubén Santos
 * @date: 17/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;
class UPawnMovementComponent;

UCLASS()
class PROJECTSINGULARITY_API UBaseAnimInstance : public UAnimInstance
{
  GENERATED_BODY()

public:
  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Reference, DisplayName = "fireMontage",
            meta = (AllowPrivateAccess = "true"))
  UAnimMontage* fireMontage;

protected:
  /** Player Ground Speed when he's on the walk or run state */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "GroundSpeed",
            meta = (AllowPrivateAccess = "true"))
  float m_groundSpeed;

private:
  UBaseAnimInstance* m_attachedAnimInstance;
  TObjectPtr<ABaseCharacter> m_baseCharacter;
  UPawnMovementComponent* m_movementComponent;

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "Direction",
            meta = (AllowPrivateAccess = "true"))
  FVector2D m_direction; //-1 left | 1 right

  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "Speed",
            meta = (AllowPrivateAccess = "true"))
  float m_speed;

  /** Player Air Speed when he's in the falling, jumping or climbing state */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "AirSpeed",
            meta = (AllowPrivateAccess = "true"))
  float m_airSpeed;

  /** Check if Player is on falling state */
  UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "AimVertical",
            meta = (AllowPrivateAccess = "true"))
  float m_aimVertical;

public:
  virtual void NativeInitializeAnimation() override;
  virtual void NativeUpdateAnimation(float DeltaSeconds) override;

  /** Function to manage ground speed variable */
  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void UpdateGroundSpeed();

  /** Function to manage air speed variable */
  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void UpdateAirSpeed();

  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void UpdateDirection();

  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void UpdateAimVertical();

  UFUNCTION(BlueprintCallable, Category = "Animation Flow")
  void Fire();

  TObjectPtr<ABaseCharacter> GetBaseCharacter();
};
