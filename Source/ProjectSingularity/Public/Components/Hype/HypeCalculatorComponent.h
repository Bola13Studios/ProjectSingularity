/************************************************************************
 * @description: Calculates the final hype to add to the player
 * @author: Josephine Esposito
 * @date: 31/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HypeCalculatorComponent.generated.h"

class UPopularityComponent;
class UHypeModifierComponent;

/**
 * Calculates the final hype to add to the player
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTSINGULARITY_API UHypeCalculatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHypeCalculatorComponent();

 	float ApplyModifiers(UHypeModifierComponent* _modifier);

	float ApplyPopularity(UPopularityComponent* _popularity);

	int ComputeFinalHype(float& _baseHype, float& _modifiers, float& _popularity);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
