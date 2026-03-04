/************************************************************************
 * @description: Represents the Hype Component
 * @author: Josephine Esposito
 * @date: 04/03/2026
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HypeComponent.generated.h"

#pragma region | Forward Declarations
class UBoxComponent;
#pragma endregion

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSINGULARITY_API UHypeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief Applyed when custom getting the final points
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Values", meta = (DisplayName = "Multiplier"))
	float Multiplier;
	
	/**
	 * @brief The amount of hype this component stores
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Values", meta = (DisplayName = "Points"))
	int Points;

public:
	
	/**
	 * @brief Sets default values for this component's properties
	 */
	UHypeComponent();

	/**
	 * @brief Add set amount of points to the existing saved ones
	 * @param _Points The amount to add to
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Hype")
	void AddPoints(const int& _Points);

	/**
	 * @brief Returns the calculated points after multiplier is applied
	 * @return a rounded int
	 */
	UFUNCTION(BlueprintPure, Category = "Bola 13|Hype")
	int GetPoints() const;

protected:
	/**
	 * @brief Called when the game starts
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief Called every frame
	 * @param DeltaTime 
	 * @param TickType 
	 * @param ThisTickFunction 
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
