/************************************************************************
 * @description: Represents the Hype Component for the Receivers such as 
								 the player
 * @author: Josephine Esposito
 * @date: 05/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "HypeComponent.h"
#include "HypeReceiverComponent.generated.h"

#pragma region | Forward Declaration
class UHypeSourceComponent;
class UDataTable; //struct FHypeMultipliers;
#pragma endregion

/**
 * @brief Represents the Hype Component for the Receivers such as the player
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UHypeReceiverComponent : public UHypeComponent
{
	GENERATED_BODY()

public:

	/**
	 * @brief Holds the current hype level for this receiver
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Hype Level"))
	int CurrentHypeLevel;

	/**
	 * @brief Hold the current kill streak for this receiver
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Hype", meta = (DisplayName = "Kill Streak"))
	int CurrentKillStreak;

private:

	UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Multiplier Table"))
	UDataTable* HypeMultiplierTable;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Hype", meta = (DisplayName = "Level Table"))
	UDataTable* HypeLevelTable;

public:

	/**
	 * @brief Used to register a kill
	 * @param _Source the killed source
	 * @param Critical if it was a critical hit
	 * @param MultiKill if it was a multi kill or not
	 */
	UFUNCTION(BlueprintCallable)
	void RegisterKill(UHypeSourceComponent* _Source, const bool& Critical, const int& MultiKill);

private:
	/**
	 * @brief Used to Update the Current Hype Level
	 */
	UFUNCTION(BlueprintCallable)
	void UpdateHypeLevel();
};
