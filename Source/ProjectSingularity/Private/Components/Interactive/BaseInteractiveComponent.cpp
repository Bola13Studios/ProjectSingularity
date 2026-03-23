#include "ProjectSingularity/Public/Components/Interactive/BaseInteractiveComponent.h"
#include "ProjectSingularity/Public/Gameplay/Character/Player/PlayerCharacter.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"

void UBaseInteractiveComponent::Interact()
{
  UE_LOG(LogTemp, Warning, TEXT("INTERACTION!!"));
}

void UBaseInteractiveComponent::OnInteractBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void UBaseInteractiveComponent::OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}

void UBaseInteractiveComponent::BeginPlay()
{
  Super::BeginPlay();

  OnComponentBeginOverlap.AddDynamic(this, &UBaseInteractiveComponent::OnInteractBeginOverlap);
  OnComponentEndOverlap.AddDynamic(this, &UBaseInteractiveComponent::OnInteractEndOverlap);
}

void UBaseInteractiveComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  OnComponentBeginOverlap.RemoveAll(this);
  OnComponentEndOverlap.RemoveAll(this);
}
//EOF