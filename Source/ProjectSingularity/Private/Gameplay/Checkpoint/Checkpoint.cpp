#include "Gameplay/Checkpoint/Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Gameplay/Character/PACharacter.h"
#include "DrawDebugHelpers.h"


ACheckpoint* ACheckpoint::m_activeCheckpoint = nullptr;
ACheckpoint::ACheckpoint()
{
    PrimaryActorTick.bCanEverTick = true;

    triggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    RootComponent = triggerBox;

    triggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::OnOverlapBegin);
    triggerBox->SetGenerateOverlapEvents(true);
}

void ACheckpoint::BeginPlay()
{
    Super::BeginPlay();

}

void ACheckpoint::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    DrawGizmos();
}


void ACheckpoint::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    ACharacter* character = Cast<ACharacter>(OtherActor);
    if (character && OtherActor != this)
    {
        m_activeCheckpoint = this;
    }
}

void ACheckpoint::RespawnPlayer()
{
    if (m_activeCheckpoint != NULL)
    {
        m_activeCheckpoint->RespawnPlayerToActiveCheckpoint();
    }
}

void ACheckpoint::RespawnPlayerToActiveCheckpoint()
{
    ACharacter* playerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (playerCharacter)
    {
        playerCharacter->TeleportTo(GetActorLocation(), GetActorRotation());
    }
}

void ACheckpoint::DrawGizmos()
{
    if (bShowGizmo)
    {
        FColor color = FColor::Magenta;
        if (m_activeCheckpoint == this)
        {
            color = FColor::Green;
        }
        DrawDebugBox(GetWorld(), triggerBox->GetComponentLocation(), triggerBox->GetScaledBoxExtent(), color, false, 0, 0, 2);
    }
}
