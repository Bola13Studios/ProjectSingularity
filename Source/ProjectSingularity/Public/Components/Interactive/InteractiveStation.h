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

enum class EStationStates : uint8;

/**
 * Represents the Station Interactive Component
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent), Blueprintable)
class PROJECTSINGULARITY_API UInteractiveStation : public UBaseInteractiveComponent
{
  GENERATED_BODY()

public:
  /**
   * @brief Represents the amount of hype it costs
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Data",
            meta = (DisplayName = "Price", ClampMin = 1))
  float hypeCost;

  /**
   * @brief Represents the amount to give
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Data", meta = (DisplayName = "Amount", ClampMin = 1))
  float amount;

  /**
   * @brief Sets the type to sell
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Data", meta = (DisplayName = "Station Mode"))
  EStationStates m_stationMode;

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
  virtual void OnInteractBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult) override;

  /**
   * @brief Custom OnEndOverlap method, verifies if it was the player
   * @param OverlappedComponent
   * @param OtherActor
   * @param OtherComp
   * @param OtherBodyIndex
   */
  virtual void OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

private:
  /**
   * @brief Will change the player's heath
   * @param _amount 
   * @param _cost 
   * @return 
   */
  bool ChangeHealth(const float& _amount, const float& _cost);

  /**
   * @brief Will change the amount of stored ammo
   * @param _amount 
   * @param _cost 
   * @return 
   */
  bool ChangeAmmo(const float& _amount, const float& _cost);

  /**
   * @brief Will change the max amount of health
   * @param _amount 
   * @param _cost 
   * @return 
   */
  bool ChangeMaxHealth(const float& _amount, const float& _cost);

  /**
   * @brief Will change the amount of damage for both weapons
   * @param _amount 
   * @param _cost 
   * @return 
   */
  bool ChangeDamage(const float& _amount, const float& _cost);
};
