#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectSingularity/Public/Data/DataAsset/PlayerConfigDataAsset.h"
#include "ProjectSingularity/Public/Systems/GameManagerSubsystem.h"
#include "ProjectSingularity/Public/Components/Hype/HypeReceiverComponent.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Weapons/WeaponsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActionStateFilter.h"
#include "Gameplay/Animation/Player/PlayerAnimInstance.h"

APlayerCharacter::APlayerCharacter()
    : ABaseCharacter()
{
  m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
  m_actionsFilterComponent = CreateDefaultSubobject<UActionStateFilter>(TEXT("ActionsStateFilter"));

  if (IsValid(m_camera))
  {
    m_camera->SetupAttachment(RootComponent);
    m_camera->bUsePawnControlRotation = true;
  }

  m_armsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
  m_armsMesh->SetupAttachment(GetCapsuleComponent());
  m_armsMesh->CastShadow = false;
  m_armsMesh->SetupAttachment(m_camera);
}

void APlayerCharacter::BeginPlay()
{
  Super::BeginPlay();

  UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
  if (IsValid(m_playerDataAsset) && IsValid(charMoveComp))
  {
    charMoveComp->MaxWalkSpeed = m_playerDataAsset->maxWalkSpeed;
    charMoveComp->JumpZVelocity = m_playerDataAsset->jumpZVelocity;
    charMoveComp->AirControl = m_playerDataAsset->airControl;
    charMoveComp->GravityScale = m_playerDataAsset->gravityScale;
  }

  FActorSpawnParameters spawnParams;
  spawnParams.Owner = this;

  m_currentWeapon = GetWorld()->SpawnActor<AWeaponBase>(m_weaponClass, spawnParams);

  if (IsValid(m_currentWeapon))
  {
    m_currentWeapon->AttachToComponent(m_armsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                                       TEXT("HandGrip_R"));            // Temp bone name
    m_currentWeapon->SetWeaponData(m_weaponDataAsset->weaponsData[0]); // Just for now
  }
  
  if (APlayerController* playerController = Cast<APlayerController>(GetController()))
  {
    if (AGameHUDSetUp* hud = Cast<AGameHUDSetUp>(playerController->GetHUD()))
    {
      if (hud->m_hudWidget && m_currentWeapon)
      {
        hud->m_hudWidget->BindToWeapon(m_currentWeapon);
      }
    }
  }

  if (UCapsuleComponent* capsuleComp = GetCapsuleComponent())
  {
    capsuleComp->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnComponentHit);
  }

  if (IsValid(m_actionsFilterComponent))
  {
    m_actionsFilterComponent->InitializeFilter(this, m_characterStatesDataAsset, UGroundMovementState::StaticClass());
  }
}

void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  float totalHype = 0.0f;
  if (UHypeReceiverComponent* hypeComp = GetComponentByClass<UHypeReceiverComponent>())
  {
    totalHype = hypeComp->GetHype();
  }

  if (UGameInstance* gameInstance = GetGameInstance())
  {
    if (UGameManagerSubsystem* gameManager = gameInstance->GetSubsystem<UGameManagerSubsystem>())
    {
      gameManager->AddStat("totalhype", totalHype);
    }
  }
}

void APlayerCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (m_bFire && m_currentWeapon)
  {
    m_bFire = m_currentWeapon->Fire();
  }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
  {
    enhancedInputComponent->BindAction(m_moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
    enhancedInputComponent->BindAction(m_jumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpAction);
    enhancedInputComponent->BindAction(m_lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAction);
    enhancedInputComponent->BindAction(m_dashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DashAction);
    enhancedInputComponent->BindAction(m_fireAction, ETriggerEvent::Started, this, &APlayerCharacter::StartFireAction);
    enhancedInputComponent->BindAction(m_fireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFireAction);
    enhancedInputComponent->BindAction(m_changeWeaponModeAction, ETriggerEvent::Started, this,
                                       &APlayerCharacter::ChangeWeaponMode);
    enhancedInputComponent->BindAction(m_reloadAction, ETriggerEvent::Started, this, &APlayerCharacter::TryToReload);
    enhancedInputComponent->BindAction(m_interactAcion, ETriggerEvent::Triggered, this,
                                       &APlayerCharacter::InteractAction);
  }
}

void APlayerCharacter::MoveAction(const FInputActionValue& _inputValue)
{
  if (IsValid(m_actionsFilterComponent))
  {
    m_actionsFilterComponent->StateAction(_inputValue);
  }
}

void APlayerCharacter::MoveInternal(const FVector2D& _inputVector)
{
  if (IsValid(Controller))
  {
    const FRotator rotation = Controller->GetControlRotation();
    const FRotator yawRotation(0, rotation.Yaw, 0);

    const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
    const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(forwardDirection, _inputVector.Y);
    AddMovementInput(rightDirection, _inputVector.X);
  }
}

void APlayerCharacter::JumpAction()
{
  RequestChangeState(UJumpingState::StaticClass());

  if (UGameInstance* gameInstance = GetGameInstance())
  {
    if (UGameManagerSubsystem* gameManager = gameInstance->GetSubsystem<UGameManagerSubsystem>())
    {
      gameManager->AddStat("jumps", 0.5f);
    }
  }
}

void APlayerCharacter::LookAction(const FInputActionValue& _inputValue)
{
  FVector2D inputVector = _inputValue.Get<FVector2D>();
  if (IsValid(Controller))
  {
    AddControllerYawInput(inputVector.X);
    AddControllerPitchInput(inputVector.Y);
  }
}

void APlayerCharacter::StartFireAction(const FInputActionValue& Value)
{
  m_bFire = true;
}

void APlayerCharacter::StopFireAction()
{
  m_bFire = false;
}

void APlayerCharacter::ChangeWeaponMode()
{
  m_currentWeapon->TryToChangeMode();
}

void APlayerCharacter::TryToReload()
{
  m_currentWeapon->TryToReload();
}

void APlayerCharacter::InteractAction(const FInputActionValue& _Value)
{ // only broadcasting the delegate
  m_onInteract.Broadcast();

  if (UGameInstance* gameInstance = GetGameInstance())
  {
    if (UGameManagerSubsystem* gameManager = gameInstance->GetSubsystem<UGameManagerSubsystem>())
    {
      gameManager->AddStat("interactions");
    }
  }
}

void APlayerCharacter::DashAction()
{
  if (m_bCanDash)
  {
    RequestChangeState(UDashingState::StaticClass());
  }
}

void APlayerCharacter::DashEnd()
{
  GetWorldTimerManager().ClearTimer(m_dashStopTimerHandle);

  IsGrounded() ? RequestChangeState(UGroundMovementState::StaticClass())
               : RequestChangeState(UFallingState::StaticClass());
}

void APlayerCharacter::Dash()
{
  if ((m_playerDataAsset->dashDistance <= 0.f) || (m_playerDataAsset->dashTime <= 0.f))
  {
    return;
  }

  if (IsValid(m_playerDataAsset) && IsValid(m_camera))
  {
    FVector dashDir = GetVelocity() * FVector(1, 1, 0);
    dashDir = dashDir.IsNearlyZero() ? m_camera->GetForwardVector() : GetLastMovementInputVector().GetSafeNormal();

    FVector dashVelocity = dashDir.GetSafeNormal() * (m_playerDataAsset->dashDistance / m_playerDataAsset->dashTime);
    dashVelocity.Z = 0.;

    m_bCanDash = false;
    if (UCharacterMovementComponent* charMoveComp = GetCharacterMovement())
    {
      charMoveComp->GravityScale = 0.f;
      charMoveComp->GroundFriction = 0.f;
    }

    LaunchCharacter(dashVelocity, true, true);

    GetWorldTimerManager().SetTimer(m_dashStopTimerHandle, this, &APlayerCharacter::DashEnd,
                                    m_playerDataAsset->dashTime);

    if (UGameInstance* gameInstance = GetGameInstance())
    {
      if (UGameManagerSubsystem* gameManager = gameInstance->GetSubsystem<UGameManagerSubsystem>())
      {
        gameManager->AddStat("dashes", 0.5f);
      }
    }
  }
}

void APlayerCharacter::StopDash()
{
  UCharacterMovementComponent* charMoveComp = GetCharacterMovement();

  if (IsValid(m_playerDataAsset) && IsValid(charMoveComp))
  {
    charMoveComp->GravityScale = m_playerDataAsset->gravityScale;
    charMoveComp->GroundFriction = m_playerDataAsset->groundFriction;
    GetWorldTimerManager().SetTimer(m_dashResetTimerHandle, this, &APlayerCharacter::ResetDash,
                                    m_playerDataAsset->dashCooldown);
  }
  else
  {
    ResetDash();
  }
}

void APlayerCharacter::ResetDash()
{
  m_bCanDash = true;
}

void APlayerCharacter::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                                      FVector NormalImpulse, const FHitResult& Hit)
{
  if (IsValid(m_actionsFilterComponent)
      && m_actionsFilterComponent->GetCurrentBaseStateClass() == UDashingState::StaticClass())
  {
    DashEnd();
  }
}

bool APlayerCharacter::IsGrounded() const
{
  FHitResult hit;

  FVector start = GetActorLocation();
  float traceDistance = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f;
  FVector end = start - FVector(0, 0, traceDistance);

  FCollisionQueryParams params;
  params.AddIgnoredActor(this);
  if (GetWorld())
  {
    return GetWorld()->LineTraceSingleByChannel(hit, start, end, ECC_WorldStatic, params);
  }
  return false;
}

void APlayerCharacter::RequestChangeState(const TSubclassOf<UStates> _state)
{
  if (IsValid(m_actionsFilterComponent))
  {
    m_actionsFilterComponent->SetCurrentState(_state);
  }
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
  Super::Landed(Hit);
  RequestChangeState(UGroundMovementState::StaticClass());
}

USkeletalMeshComponent* APlayerCharacter::GetArmsMesh()
{
  return m_armsMesh;
}

void APlayerCharacter::ShowDebugsWeapon(bool value)
{
  m_bDebugWeapon = value;
}

bool APlayerCharacter::GetDebugWeapon()
{
  return m_bDebugWeapon;
}

TObjectPtr<UPlayerAnimInstance> APlayerCharacter::GetPlayerAnimInstance()
{
  if (m_playerAnimInstance == NULL)
  {
    m_playerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
  }

  return m_playerAnimInstance;
}

TObjectPtr<AWeaponBase> APlayerCharacter::GetWeapon() const
{
  return m_currentWeapon;
}
