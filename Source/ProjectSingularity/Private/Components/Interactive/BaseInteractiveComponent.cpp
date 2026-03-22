#include "ProjectSingularity/Public/Components/Interactive/BaseInteractiveComponent.h"
#include "ProjectSingularity/Public/Gameplay/Character/Player/PlayerCharacter.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"

void UBaseInteractiveComponent::Interact()
{
  UE_LOG(LogTemp, Warning, TEXT("INTERACTION!!"));

  if (!IsValid(m_Player))
  {
    return;
  }

  // --- health component --- //
  UHealthComponent* HealthComponent = m_Player->FindComponentByClass<UHealthComponent>();

  // --- hype component   --- //
  UHypeReceiverComponent* HypeComponent = m_Player->FindComponentByClass<UHypeReceiverComponent>();

  // we verify if both the components are present on the player
  if (!IsValid(HealthComponent) || !IsValid(HypeComponent))
  {
    UE_LOG(LogTemp, Error, TEXT("MISSING REQUIRED COMPONENTS ON PLAYER"));
    return;
  }

  const float HealAmount = 20.0f;
  const float HypeCost = 10.0f;

  // we check if the player has enough hype
  if (!HypeComponent->IsHypeEnough(HypeCost))
  {
    UE_LOG(LogTemp, Warning, TEXT("Not enough Hype :("));
    return;
  }

  HypeComponent->AddHype(-HypeCost);
  HealthComponent->ChangeHealth(HealAmount, GetOwner());

  UE_LOG(LogTemp, Warning, TEXT("Changed health to -> %f"), HealthComponent->GetHealth());
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