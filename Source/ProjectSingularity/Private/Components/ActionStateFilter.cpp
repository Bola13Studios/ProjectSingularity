

#include "Components/ActionStateFilter.h"
#include "Gameplay/Character/BaseCharacter.h"

UActionStateFilter::UActionStateFilter()
{
	PrimaryComponentTick.bCanEverTick = true;


}


// Called when the game starts
void UActionStateFilter::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UActionStateFilter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

