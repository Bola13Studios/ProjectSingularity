#include "Gameplay/Character/Player/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectSingularity/Public/Data/DataAsset/PlayerConfigDataAsset.h"
#include "Gameplay/Weapons/WeaponBase.h"
#include "Gameplay/Weapons/WeaponsDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Components/ActionStateFilter.h"

APlayerCharacter::APlayerCharacter()
    : ABaseCharacter()
{
  m_Camera                 = CreateDefaultSubobject<UCameraComponent>(TEXT("Player Camera"));
  m_ActionsFilterComponent = CreateDefaultSubobject<UActionStateFilter>(TEXT("ActionsStateFilter"));

  if (IsValid(m_Camera))
  {
    m_Camera->SetupAttachment(RootComponent);
    m_Camera->bUsePawnControlRotation = true;
  }

  m_ArmsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArmsMesh"));
  m_ArmsMesh->SetupAttachment(GetCapsuleComponent());
  m_ArmsMesh->CastShadow = false;
  m_ArmsMesh->SetupAttachment(m_Camera);
}

void APlayerCharacter::BeginPlay()
{
  Super::BeginPlay();

  UCharacterMovementComponent* charMoveComp = GetCharacterMovement();
  if (IsValid(m_PlayerDataAsset) && IsValid(charMoveComp))
  {
    charMoveComp->MaxWalkSpeed  = m_PlayerDataAsset->maxWalkSpeed;
    charMoveComp->JumpZVelocity = m_PlayerDataAsset->jumpZVelocity;
    charMoveComp->AirControl    = m_PlayerDataAsset->airControl;
    charMoveComp->GravityScale  = m_PlayerDataAsset->gravityScale;
  }

  FActorSpawnParameters spawnParams;
  spawnParams.Owner = this;

  m_CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(m_WeaponClass, spawnParams);

  if (IsValid(m_CurrentWeapon))
  {
    m_CurrentWeapon->AttachToComponent(m_ArmsMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                                       TEXT("HandGrip_R"));            // Temp bone name
    m_CurrentWeapon->SetWeaponData(m_WeaponDataAsset->weaponsData[0]); // Just for now
  }

  if (UCapsuleComponent* capsuleComp = GetCapsuleComponent())
  {
    capsuleComp->OnComponentHit.AddDynamic(this, &APlayerCharacter::OnComponentHit);
  }

  if (IsValid(m_ActionsFilterComponent))
  {
    m_ActionsFilterComponent->InitializeFilter(this, m_CharacterStatesDataAsset, UGroundMovementState::StaticClass());
  }
}

void APlayerCharacter::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (m_bFire && m_CurrentWeapon)
  {
    m_bFire = m_CurrentWeapon->Fire();
  }
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
  Super::SetupPlayerInputComponent(PlayerInputComponent);

  if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
  {
    enhancedInputComponent->BindAction(m_MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveAction);
    enhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::JumpAction);
    enhancedInputComponent->BindAction(m_LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::LookAction);
    enhancedInputComponent->BindAction(m_DashAction, ETriggerEvent::Triggered, this, &APlayerCharacter::DashAction);
    enhancedInputComponent->BindAction(m_FireAction, ETriggerEvent::Started, this, &APlayerCharacter::StartFireAction);
    enhancedInputComponent->BindAction(m_FireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFireAction);
    enhancedInputComponent->BindAction(m_ChangeWeaponModeAction, ETriggerEvent::Started, this,
                                       &APlayerCharacter::ChangeWeaponMode);
    enhancedInputComponent->BindAction(m_ReloadAction, ETriggerEvent::Started, this, &APlayerCharacter::TryToReload);
    enhancedInputComponent->BindAction(m_InteractAcion, ETriggerEvent::Triggered, this,
                                       &APlayerCharacter::InteractAction);
  }
}

void APlayerCharacter::MoveAction(const FInputActionValue& _inputValue)
{
  if (IsValid(m_ActionsFilterComponent))
  {
    m_ActionsFilterComponent->StateAction(_inputValue);
  }
}

void APlayerCharacter::MoveInternal(const FVector2D& _inputVector)
{
  if (IsValid(Controller))
  {
    const FRotator rotation = Controller->GetControlRotation();
    const FRotator yawRotation(0, rotation.Yaw, 0);

    const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
    const FVector rightDirection   = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
    AddMovementInput(forwardDirection, _inputVector.Y);
    AddMovementInput(rightDirection, _inputVector.X);
  }
}

void APlayerCharacter::JumpAction()
{
  RequestChangeState(UJumpingState::StaticClass());
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
  m_CurrentWeapon->TryToChangeMode();
}

void APlayerCharacter::TryToReload()
{
  m_CurrentWeapon->TryToReload();
}

void APlayerCharacter::InteractAction(const FInputActionValue& _Value)
{ // only broadcasting the delegate
  m_OnInteract.Broadcast();
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
  GetWorldTimerManager().ClearTimer(m_DashStopTimerHandle);

  IsGrounded() ? RequestChangeState(UGroundMovementState::StaticClass())
               : RequestChangeState(UFallingState::StaticClass());
}

void APlayerCharacter::Dash()
{
  if ((m_PlayerDataAsset->dashDistance <= 0.f) || (m_PlayerDataAsset->dashTime <= 0.f))
  {
    return;
  }

  if (IsValid(m_PlayerDataAsset) && IsValid(m_Camera))
  {
    FVector dashDir = GetVelocity() * FVector(1, 1, 0);
    dashDir = dashDir.IsNearlyZero() ? m_Camera->GetForwardVector() : GetLastMovementInputVector().GetSafeNormal();

    FVector dashVelocity = dashDir.GetSafeNormal() * (m_PlayerDataAsset->dashDistance / m_PlayerDataAsset->dashTime);
    dashVelocity.Z       = 0.;

    m_bCanDash = false;
    if (UCharacterMovementComponent* charMoveComp = GetCharacterMovement())
    {
      charMoveComp->GravityScale   = 0.f;
      charMoveComp->GroundFriction = 0.f;
    }

    LaunchCharacter(dashVelocity, true, true);

    GetWorldTimerManager().SetTimer(m_DashStopTimerHandle, this, &APlayerCharacter::DashEnd,
                                    m_PlayerDataAsset->dashTime);
  }
}

void APlayerCharacter::StopDash()
{
  UCharacterMovementComponent* charMoveComp = GetCharacterMovement();

  if (IsValid(m_PlayerDataAsset) && IsValid(charMoveComp))
  {
    charMoveComp->GravityScale   = m_PlayerDataAsset->gravityScale;
    charMoveComp->GroundFriction = m_PlayerDataAsset->groundFriction;
    GetWorldTimerManager().SetTimer(m_DashResetTimerHandle, this, &APlayerCharacter::ResetDash,
                                    m_PlayerDataAsset->dashCooldown);
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
  if (IsValid(m_ActionsFilterComponent)
      && m_ActionsFilterComponent->GetCurrentBaseStateClass() == UDashingState::StaticClass())
  {
    DashEnd();
  }
}

bool APlayerCharacter::IsGrounded() const
{
  FHitResult hit;

  FVector start         = GetActorLocation();
  float   traceDistance = GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 5.f;
  FVector end           = start - FVector(0, 0, traceDistance);

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
  if (IsValid(m_ActionsFilterComponent))
  {
    m_ActionsFilterComponent->SetCurrentState(_state);
  }
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
  Super::Landed(Hit);
  RequestChangeState(UGroundMovementState::StaticClass());
}

USkeletalMeshComponent* APlayerCharacter::GetArmsMesh()
{
  return m_ArmsMesh;
}

void APlayerCharacter::ShowDebugsWeapon(bool value)
{
  m_bDebugWeapon = value;
}

bool APlayerCharacter::GetDebugWeapon()
{
  return m_bDebugWeapon;
}
