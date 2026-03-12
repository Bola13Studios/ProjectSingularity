/************************************************************************
 * @description: Represents the Hype Component for the source such as 
								 the enemies or other objects
 * @author: Josephine Esposito
 * @date: 05/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeComponent.h"
#include "HypeSourceComponent.generated.h"

/**
 * @brief Represents the Hype Component for the source such as 
 * the enemies or other objects
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UHypeSourceComponent : public UHypeComponent
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief A Custom multiplier to apply to the base value
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bola 13|Hype", meta = (DisplayName = "Custom Multiplier"))
	float BaseMultiplier;

	/**
	 * @brief Returns the calculated points after multiplier is applied
	 * @return a rounded int
	 */
	int GetHype() const override;
};
