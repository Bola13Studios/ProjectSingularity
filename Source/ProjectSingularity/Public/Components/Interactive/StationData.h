/************************************************************************
 * @description: Represents the structure for the Station Data Table
 * @author: Josephine Esposito
 * @date: 22/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "StationStates.h"
#include "StationData.generated.h"

/**
 * Represents the structure for the Station Data Table
 */
USTRUCT(BlueprintType)
struct FStationData : public FTableRowBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Allowed Type"))
	EStationStates m_Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Amount Type"))
	float m_TypeAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data", meta = (DisplayName = "Type Price"))
	float m_TypePrice;

};