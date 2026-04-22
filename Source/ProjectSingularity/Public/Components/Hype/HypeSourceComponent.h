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

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Hype", meta = (DisplayName = "Is it a Weakpoint?"))
  bool m_isWeakPoint;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Hype", meta = (DisplayName = "Popularity Value"))
  int32 popularityValue;

public:
  /**
   * @brief Returns the base hype value
   * @return a rounded int
   */
  int GetHype() override;
};
