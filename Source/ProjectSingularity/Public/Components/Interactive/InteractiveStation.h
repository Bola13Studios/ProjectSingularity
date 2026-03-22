/************************************************************************
 * @description: Represents the Station Interactive Component
 * @author: Josephine Esposito
 * @date: 20/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/Interactive/BaseInteractiveComponent.h"
#include "InteractiveStation.generated.h"

/**
 * Represents the Station Interactive Component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECTSINGULARITY_API UInteractiveStation : public UBaseInteractiveComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief This holds all the prices and amount to add to the related categories
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Data", meta = (DisplayName = "Data Table"))
	UDataTable* m_StationData;

public:

	/**
	 * @brief Base interact method
	 */
	virtual void Interact() override;

	/**
	 * @brief Custom OnBegiOverlap method, verifies if was the player
	 * @param OverlappedComponent
	 * @param OtherActor
	 * @param OtherComp
	 * @param OtherBodyIndex
	 * @param bFromSweep
	 * @param SweepResult
	 */
	virtual void OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	/**
	 * @brief Custom OnEndOverlap method, verifies if it was the player
	 * @param OverlappedComponent
	 * @param OtherActor
	 * @param OtherComp
	 * @param OtherBodyIndex
	 */
	virtual void OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:

	bool ChangeHealth(const float& _Amount, const float& _Cost);

	bool ChangeAmmo(const float& _Amount, const float& _Cost);
};
