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
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


	/** Function to manage ground speed variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateGroundSpeed();

	/** Function to manage air speed variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateAirSpeed();

	/** Function to manage the should move boolean variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateShouldMove();

	/** Function to manage the is falling boolean variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateIsFalling();

	/** Function to manage the is jumping boolean variable */
	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateIsJumping();

	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateDirection();

	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void UpdateAimVertical();

	UFUNCTION(BlueprintCallable, Category = "Animation Flow")
	void Fire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Reference, DisplayName = "fireMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* fireMontage;

private:

	UBaseAnimInstance* m_attachedAnimInstance;

	ABaseCharacter* m_baseCharacter;
	UPawnMovementComponent* m_movementComponent;


	/** Player Ground Speed when he's on the walk or run state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "GroundSpeed", meta = (AllowPrivateAccess = "true"))
	float m_GroundSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "Direction", meta = (AllowPrivateAccess = "true"))
	FVector2D m_Direction; //-1 left | 1 right

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "Speed", meta = (AllowPrivateAccess = "true"))
	float m_Speed;

	/** Player Air Speed when he's in the falling, jumping or climbing state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "AirSpeed", meta = (AllowPrivateAccess = "true"))
	float m_AirSpeed;

	/** Check if Player should move when he's on the idle state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "ShouldMove", meta = (AllowPrivateAccess = "true"))
	bool m_ShouldMove;

	/** Check if Player is on falling state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "IsFalling", meta = (AllowPrivateAccess = "true"))
	bool m_IsFalling;

	/** Check if Player is on falling state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "IsJumping", meta = (AllowPrivateAccess = "true"))
	bool m_IsJumping;

	/** Check if Player is on falling state */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Reference, DisplayName = "AimVertical", meta = (AllowPrivateAccess = "true"))
	float m_aimVertical;

};
