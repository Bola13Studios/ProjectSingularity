/************************************************************************
 * @description: Represents the Base Hype Component
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

/**
 * @brief The base Hype Component
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSINGULARITY_API UHypeComponent : public UActorComponent
{
	GENERATED_BODY()

protected:	
	/**
	 * @brief The amount of hype this component stores
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Base Hype Value"))
	int BaseHypeValue;

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
	virtual void AddHype(const int& _Points);

	/**
	 * @brief Returns the calculated points after multiplier is applied
	 * @return a rounded int
	 */
	UFUNCTION(BlueprintPure, Category = "Bola 13|Hype")
	virtual int GetHype() const;

	/**
	 * @brief Called to reset current points back to 0
	 */
	UFUNCTION(BlueprintCallable, Category = "Bola 13|Hype")
	void ResetHype();
};
//EOF