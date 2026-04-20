#include "ProjectSingularity/Public/UI/AmmoWidget.h"

#include "Components/TextBlock.h"
#include "Gameplay/Weapons/WeaponBase.h"

#pragma region | PUBLIC METHODS

void UAmmoWidget::BindToWeapon(AWeaponBase* _weapon)
{
  if (m_weapon)
  {
    m_weapon->OnReserveAmmoChanged.RemoveDynamic(this, &UAmmoWidget::HandleReserveAmmoChanged);
    m_weapon->OnFirstModeAmmoChanged.RemoveDynamic(this, &UAmmoWidget::HandleFirstModeAmmoChanged);
    m_weapon->OnSecondModeAmmoChanged.RemoveDynamic(this, &UAmmoWidget::HandleSecondModeAmmoChanged);
    m_weapon->OnCurrentWeaponModeChanged.RemoveDynamic(this, &UAmmoWidget::HandleCurrentModeChanged);
  }

  m_weapon = _weapon;

  if (!m_weapon)
  {
    return;
  }

  m_weapon->OnReserveAmmoChanged.AddDynamic(this, &UAmmoWidget::HandleReserveAmmoChanged);
  m_weapon->OnFirstModeAmmoChanged.AddDynamic(this, &UAmmoWidget::HandleFirstModeAmmoChanged);
  m_weapon->OnSecondModeAmmoChanged.AddDynamic(this, &UAmmoWidget::HandleSecondModeAmmoChanged);
  m_weapon->OnCurrentWeaponModeChanged.AddDynamic(this, &UAmmoWidget::HandleCurrentModeChanged);

  m_reserveAmmo = m_weapon->GetAmmoInReserve();
  m_currentMode = m_weapon->GetCurrentWeaponMode();

  if (m_currentMode == EWeaponMode::ShortDistance)
  {
    m_currentAmmo = m_weapon->GetFirstModeAmmoInMagazine();
    m_maxAmmo = m_weapon->GetFirstModeMaxAmmoInMagazine();
  }
  else
  {
    m_currentAmmo = m_weapon->GetSecondModeAmmoInMagazine();
    m_maxAmmo = m_weapon->GetSecondModeMaxAmmoInMagazine();
  }

  RefreshUI();
}

#pragma endregion

#pragma region | PROTECTED METHODS

void UAmmoWidget::NativeDestruct()
{
  if (m_weapon)
  {
    m_weapon->OnReserveAmmoChanged.RemoveDynamic(this, &UAmmoWidget::HandleReserveAmmoChanged);
    m_weapon->OnFirstModeAmmoChanged.RemoveDynamic(this, &UAmmoWidget::HandleFirstModeAmmoChanged);
    m_weapon->OnSecondModeAmmoChanged.RemoveDynamic(this, &UAmmoWidget::HandleSecondModeAmmoChanged);
    m_weapon->OnCurrentWeaponModeChanged.RemoveDynamic(this, &UAmmoWidget::HandleCurrentModeChanged);
  }

  Super::NativeDestruct();
}

#pragma endregion

#pragma region | PRIVATE METHODS

void UAmmoWidget::HandleReserveAmmoChanged(int _currentReserveAmmo)
{
  m_reserveAmmo = _currentReserveAmmo;
  RefreshUI();
}

void UAmmoWidget::HandleFirstModeAmmoChanged(int _currentAmmoInMag, int _maxAmmoInMag)
{
  if (m_currentMode == EWeaponMode::ShortDistance)
  {
    m_currentAmmo = _currentAmmoInMag;
    m_maxAmmo = _maxAmmoInMag;
    RefreshUI();
  }
}

void UAmmoWidget::HandleSecondModeAmmoChanged(int _currentAmmoInMag, int _maxAmmoInMag)
{
  if (m_currentMode == EWeaponMode::LongDistance)
  {
    m_currentAmmo = _currentAmmoInMag;
    m_maxAmmo = _maxAmmoInMag;
    RefreshUI();
  }
}

void UAmmoWidget::HandleCurrentModeChanged(EWeaponMode _currentMode)
{
  m_currentMode = _currentMode;

  if (!m_weapon)
  {
    return;
  }

  if (m_currentMode == EWeaponMode::ShortDistance)
  {
    m_currentAmmo = m_weapon->GetFirstModeAmmoInMagazine();
    m_maxAmmo = m_weapon->GetFirstModeMaxAmmoInMagazine();
  }
  else
  {
    m_currentAmmo = m_weapon->GetSecondModeAmmoInMagazine();
    m_maxAmmo = m_weapon->GetSecondModeMaxAmmoInMagazine();
  }

  RefreshUI();
}

void UAmmoWidget::RefreshUI()
{
  if (m_magazineAmmoText)
  {
    m_magazineAmmoText->SetText(
      FText::FromString(FString::Printf(TEXT("%d"), m_currentAmmo))
    );
  }

  if (m_reserveAmmoText)
  {
    m_reserveAmmoText->SetText(FText::AsNumber(m_reserveAmmo));
  }

  if (m_weaponModeText)
  {
    FString modeText = TEXT("Mode Active");

    if (m_weapon)
    {
      modeText = FString::Printf(TEXT("%s"), *m_weapon->GetWeaponName());
    }

    m_weaponModeText->SetText(FText::FromString(modeText));
  }
}

#pragma endregion