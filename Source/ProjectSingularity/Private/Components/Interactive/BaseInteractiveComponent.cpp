#include "ProjectSingularity/Public/Components/Interactive/BaseInteractiveComponent.h"
#include "ProjectSingularity/Public/Gameplay/Character/Player/PlayerCharacter.h"
#include "ProjectSingularity/Public/Components/HealthComponent.h"

void UBaseInteractiveComponent::Interact()
{
  UE_LOG(LogTemp, Warning, TEXT("INTERACTION!!"));

  TArray<UHealthComponent*> Components;
  if (IsValid(m_Player))
  {
    m_Player->GetComponents<UHealthComponent>(Components);
    for (UHealthComponent* HealthComponent : Components)
    {
      if (IsValid(HealthComponent))
      { // for now the value is fixed | @reminder to make it accessible
        HealthComponent->ChangeHealth(-20, GetOwner());
        UE_LOG(LogTemp, Warning, TEXT("Changed health to -> %f"), HealthComponent->GetHealth());
      }
    }
  }
}

void UBaseInteractiveComponent::OnInteractBeginOverlap( UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  // we check if the ovelap was with the player
  if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
  {
    m_IsOverlapping = true;
    OnInteractOverlap.Broadcast(m_IsOverlapping);
    
    m_Player = Player;

    // we bind the method to the delegate
    m_Player->m_OnInteract.AddUObject(this, &UBaseInteractiveComponent::Interact);
  }
}

void UBaseInteractiveComponent::OnInteractEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  // we check if the overlap was with the player
  if (Cast<AActor>(m_Player) == OtherActor)
  {
    // we unbind the method to the delegate
    m_Player->m_OnInteract.RemoveAll(this);

    m_IsOverlapping = false;
    OnInteractOverlap.Broadcast(m_IsOverlapping);
    
    m_Player = nullptr;
  }
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