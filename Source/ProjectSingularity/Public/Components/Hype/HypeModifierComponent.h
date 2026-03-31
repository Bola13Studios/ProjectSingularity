/************************************************************************
 * @description: Handles the modifiers for the hype
 * @author: Josephine Esposito
 * @date: 31/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HypeModifierComponent.generated.h"

struct FHypeModifiers;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PROJECTSINGULARITY_API UHypeModifierComponent : public UActorComponent
{
  GENERATED_BODY()

private:
  /**
   * @brief This will hold all the current modifiers to apply
   */
  TArray<FHypeModifiers*> m_hypeModifiers;

public:
  /**
   * @brief Sets default values for this component's properties
   */
  UHypeModifierComponent();

  /**
   * @brief Will return all the saved modifiers
   * @return An array holding all the added modifiers at the time
   */
  TArray<int> GetActiveModifiers() const;

  /**
   * @brief Will add a new modifier
   * @param _modifier The key name of the modifier to add
   * @return A flag stating if the modifier was found in the related data table and addded (true)
   */
  bool AddModifier(FName _modifier);

  /**
   * @brief Will remove a specific modifier
   * @param _modifier The key name of the modifier to remove
   * @return A flag stating if the modifier was found in the array (true)
   */
  bool RemoveModifier(FName _modifier);

  /**
   * @brief Will return the total modifiers multiplier
   * @return
   */
  float GetTotalModifiers() const;

  /**
   * @brief This will clear all the temporary modifiers
   * @return
   */
  bool ClearModifiers() const;

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
  virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction* ThisTickFunction) override;
};
