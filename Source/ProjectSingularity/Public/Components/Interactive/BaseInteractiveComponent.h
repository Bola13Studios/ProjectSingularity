/************************************************************************
 * @description: Represents the Base Interactive Component
 * @author: Josephine Esposito
 * @date: 16/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "BaseInteractiveComponent.generated.h"

class APlayerCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOverlap, bool, _IsOverlapping);

/**
 * Represents the Base Interactive Component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECTSINGULARITY_API UBaseInteractiveComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief A reference to the current player
	 */
	TObjectPtr<APlayerCharacter> m_Player;

	/**
	 * @brief Used to notify whether the player overlapped with the interactable
	 */
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnOverlap OnInteractOverlap;

protected:

	/**
	 * @brief Referred flag that states when the player overlapped the interactable
	 */
	bool m_IsOverlapping;

public:

	/**
	 * @brief Base interact method
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Interactive")
	virtual void Interact();

	/**
	 * @brief Custom OnBegiOverlap method, verifies if was the player
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 * @param bFromSweep 
	 * @param SweepResult 
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Interactive")
	virtual void OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief Custom OnEndOverlap method, verifies if it was the player
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Interactive")
	virtual void OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent,	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:

	/**
	 * @brief Will register custom functions to the OnOverlap methods
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Will unregister custom functions to the OnOverlap methods
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

};
//EOF