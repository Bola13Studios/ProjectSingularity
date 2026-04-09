#include "Systems/LogManagerSubsystem.h"
#include "Systems/GameManagerSubsystem.h"
#include "Systems/BaseGameInstance.h"
#include <Misc/FileHelper.h>
#include <Misc/Paths.h>
#include <HAL/PlatformFileManager.h>
#include <Dom/JsonObject.h>
#include <Serialization/JsonWriter.h>
#include <Serialization/JsonSerializer.h>

void ULogManagerSubsystem::Initialize(FSubsystemCollectionBase& _rCollection)
{
  Super::Initialize(_rCollection);

  UE_LOG(LogTemp, Log, TEXT("Logger Initialized"));
}

void ULogManagerSubsystem::Deinitialize()
{
  if (UGameInstance* gameInstance = GetGameInstance())
  {
    if (UGameManagerSubsystem* gameManager = gameInstance->GetSubsystem<UGameManagerSubsystem>())
    {
      SaveSession(gameManager->GetAllData());
    }
  }

  SaveToFile(); // we automatically save to a file upon closure

  Super::Deinitialize();
}

void ULogManagerSubsystem::LogEvent(const FString& _event)
{
  FString timestamp = FDateTime::Now().ToString();
  m_loggedEvents.Add(FString::Printf(TEXT("[%s] %s"), *timestamp, *_event));
}

void ULogManagerSubsystem::SaveToFile()
{
  if (m_loggedEvents.Num() == 0) return; // no events to log here

  FString output = FString::Join(m_loggedEvents, TEXT("\n"));
  FString filePath = GenerateFilePath("Events") + TEXT(".txt");

  FFileHelper::SaveStringToFile(output, *filePath);

  UE_LOG(LogTemp, Log, TEXT("Saved log to: %s"), *filePath);
}

void ULogManagerSubsystem::SaveSession(const FSessionData& _data)
{
  FString output;

  for (const auto& pair : _data.stats)
  {
    output += FString::Printf(TEXT("%s : %f\n"), *pair.Key.ToString(), pair.Value);
  }

  FString filePath = GenerateFilePath("Stats") + TEXT(".txt");
  FFileHelper::SaveStringToFile(output, *filePath);
}

void ULogManagerSubsystem::SaveSessionJSON(const FSessionData& _data)
{
  TSharedPtr<FJsonObject> root = MakeShareable(new FJsonObject());

  root->SetStringField("session-id", FGuid::NewGuid().ToString());
  root->SetStringField("session-time", FDateTime::Now().ToString());
  root->SetStringField("session-duration", _data.GetArrayStat("session-duration"));
  root->SetStringField("game-version", GetVersion());

#pragma region | Combat Stats |
  /// --- COMBAT --- ///
  TSharedPtr<FJsonObject> combat = MakeShareable(new FJsonObject());
  /// --- combat upgrades --- ///
  TSharedPtr<FJsonObject> upgrades = MakeShareable(new FJsonObject());
  upgrades->SetStringField("combat-start-upgrades", _data.GetArrayStat("combat-start-upgrades"));
  upgrades->SetStringField("combat-end-upgrades", _data.GetArrayStat("combat-end-upgrades"));
  combat->SetObjectField("upgrades", upgrades);
  /// --- combat damage --- ///
  TSharedPtr<FJsonObject> damage = MakeShareable(new FJsonObject());
  damage->SetNumberField("combat-total-damage", _data.GetStat("combat-total-damage"));
  damage->SetNumberField("combat-total-damage-short-range", _data.GetStat("combat-total-damage-short-range"));
  damage->SetNumberField("combat-total-damage-long-range", _data.GetStat("combat-total-damage-long-range"));
  combat->SetObjectField("damage", damage);
  /// --- combat shots --- ///
  TSharedPtr<FJsonObject> shots = MakeShareable(new FJsonObject());
  shots->SetNumberField("combat-total-shots", _data.GetStat("combat-total-shots"));
  shots->SetNumberField("combat-total-shots-short-range", _data.GetStat("combat-total-shots-short-range"));
  shots->SetNumberField("combat-total-shots-long-range", _data.GetStat("combat-total-shots-long-range"));
  combat->SetObjectField("shots", shots);
  /// --- combat hits --- ///
  TSharedPtr<FJsonObject> hits = MakeShareable(new FJsonObject());
  hits->SetNumberField("combat-total-hits", _data.GetStat("combat-total-hits"));
  hits->SetNumberField("combat-total-hits-short-range", _data.GetStat("combat-total-hits-short-range"));
  hits->SetNumberField("combat-total-hits-long-range", _data.GetStat("combat-total-hits-long-range"));
  combat->SetObjectField("hits", hits);
  /// --- combat critical hits --- ///
  TSharedPtr<FJsonObject> crits = MakeShareable(new FJsonObject());
  crits->SetNumberField("combat-total-critical-hits", _data.GetStat("combat-total-critical-hits"));
  crits->SetNumberField("combat-total-critical-hits-short-range",
                        _data.GetStat("combat-total-critical-hits-short-range"));
  crits->SetNumberField("combat-total-critical-hits-long-range",
                        _data.GetStat("combat-total-critical-hits-long-range"));
  combat->SetObjectField("critical-hits", crits);
  /// --- combat health --- ///
  TSharedPtr<FJsonObject> health = MakeShareable(new FJsonObject());
  health->SetNumberField("combat-total-health-lost", _data.GetStat("combat-total-health-lost"));
  health->SetNumberField("combat-total-gained-health", _data.GetStat("combat-total-gained-health"));
  health->SetNumberField("combat-health-bought", _data.GetStat("combat-health-bought"));
  health->SetNumberField("combat-health-won", _data.GetStat("combat-health-won"));
  combat->SetObjectField("health", health);
  /// --- combat deaths --- ///
  // TSharedPtr<FJsonObject> deaths = MakeShareable(new FJsonObject());
  combat->SetNumberField("combat-total-deaths", _data.GetStat("combat-total-deaths"));
  /// --- combat ammo --- ///
  TSharedPtr<FJsonObject> ammo = MakeShareable(new FJsonObject());
  ammo->SetNumberField("combat-total-used-ammo", _data.GetStat("combat-total-used-ammo"));
  ammo->SetNumberField("combat-total-gained-ammo", _data.GetStat("combat-total-gained-ammo"));
  ammo->SetNumberField("combat-total-bought-ammo", _data.GetStat("combat-total-bought-ammo"));
  ammo->SetNumberField("combat-total-won-ammo", _data.GetStat("combat-total-won-ammo"));
  ammo->SetNumberField("combat-total-used-ammo-short-range", _data.GetStat("combat-total-used-ammo-short-range"));
  ammo->SetNumberField("combat-total-used-ammo-long-range", _data.GetStat("combat-total-used-ammo-long-range"));
  combat->SetObjectField("ammo", ammo);

  root->SetObjectField("Combat", combat);
#pragma endregion

#pragma region | Hype Stats |
  /// --- HYPE --- ///
  TSharedPtr<FJsonObject> hype = MakeShareable(new FJsonObject());

  hype->SetNumberField("total-gained-hype", _data.GetStat("total-gained-hype"));
  hype->SetNumberField("total-gained-hype-short-range", _data.GetStat("total-gained-hype-short-range"));
  hype->SetNumberField("total-gained-hype-long-range", _data.GetStat("total-gained-hype-long-range"));
  hype->SetNumberField("total-gained-hype-normal-hit", _data.GetStat("total-gained-hype-normal-hit"));
  hype->SetNumberField("total-gained-hype-critical-hit", _data.GetStat("total-gained-hype-critical-hit"));
  hype->SetNumberField("total-gained-hype-multikill", _data.GetStat("total-gained-hype-multikill"));
  hype->SetNumberField("total-used-hype-onhealth", _data.GetStat("total-used-hype-onhealth"));
  hype->SetNumberField("total-used-hype-onammo", _data.GetStat("total-used-hype-onammo"));
  hype->SetNumberField("total-used-hype-onupgrades", _data.GetStat("total-used-hype-onupgrades"));

  root->SetObjectField("hype", hype);
#pragma endregion

#pragma region | Slotmachine Stats |
  /// --- SLOT MACHINE --- ///
  TSharedPtr<FJsonObject> slotMachine = MakeShareable(new FJsonObject());

  slotMachine->SetNumberField("slotmachine-total-uses", _data.GetStat("slotmachine-total-uses"));
  slotMachine->SetNumberField("slotmachine-total-wins", _data.GetStat("slotmachine-total-wins"));
  slotMachine->SetNumberField("slotmachine-total-positive-wins", _data.GetStat("slotmachine-total-positive-wins"));
  slotMachine->SetNumberField("slotmachine-total-negative-wins", _data.GetStat("slotmachine-total-negative-wins"));
  slotMachine->SetNumberField("slotmachine-total-health-wins", _data.GetStat("slotmachine-total-health-wins"));
  slotMachine->SetNumberField("slotmachine-total-ammo-wins", _data.GetStat("slotmachine-total-ammo-wins"));
  slotMachine->SetNumberField("slotmachine-total-other-wins", _data.GetStat("slotmachine-total-other-wins"));
  slotMachine->SetStringField("slotmachine-start-symbols", _data.GetArrayStat("slotmachine-start-symbols"));
  slotMachine->SetStringField("slotmachine-end-symbols", _data.GetArrayStat("slotmachine-end-symbols"));

  root->SetObjectField("slotmachine", slotMachine);
#pragma endregion

#pragma region | Enemy Stats |
  /// --- ENEMIES --- ///
  TSharedPtr<FJsonObject> enemy = MakeShareable(new FJsonObject());
  /// --- chaser --- ///
  TSharedPtr<FJsonObject> chaser = MakeShareable(new FJsonObject());
  chaser->SetNumberField("enemy-total-chaser-deaths", _data.GetStat("enemy-total-chaser-deaths"));
  chaser->SetNumberField("enemy-total-chaser-critical-hits", _data.GetStat("enemy-total-chaser-critical-hits"));
  chaser->SetNumberField("enemy-total-chaser-critical-deaths", _data.GetStat("enemy-total-chaser-critical-deaths"));
  chaser->SetNumberField("enemy-total-chaser-hits", _data.GetStat("enemy-total-chaser-hits"));
  enemy->SetObjectField("chaser", chaser);
  /// --- dron --- ///
  TSharedPtr<FJsonObject> dron = MakeShareable(new FJsonObject());
  dron->SetNumberField("enemy-total-dron-deaths", _data.GetStat("enemy-total-dron-deaths"));
  dron->SetNumberField("enemy-total-dron-critical-hits", _data.GetStat("enemy-total-dron-critical-hits"));
  dron->SetNumberField("enemy-total-dron-critical-deaths", _data.GetStat("enemy-total-dron-critical-deaths"));
  dron->SetNumberField("enemy-total-dron-hits", _data.GetStat("enemy-total-dron-hits"));
  enemy->SetObjectField("dron", dron);

  root->SetObjectField("enemy", enemy);
#pragma endregion

  // we convert it to string
  FString outputString;
  TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&outputString);
  FJsonSerializer::Serialize(root.ToSharedRef(), writer);

  FString filePath = GenerateFilePath("Stats") + TEXT(".json");
  FFileHelper::SaveStringToFile(outputString, *filePath);
}

FString ULogManagerSubsystem::GenerateFilePath(FString _folder) const
{
  FString _dir = FPaths::ProjectSavedDir() + TEXT("SessionLogs/") + _folder + TEXT("/");

  IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

  if (!platformFile.DirectoryExists(*_dir))
    platformFile.CreateDirectoryTree(*_dir); // we create the dir if it does not exists

  FString _fileName = FString::Printf(TEXT("Session_%s"), *FDateTime::Now().ToString(TEXT("%Y%m%d_%H%M%S")));

  return _dir + _fileName;
}

FString ULogManagerSubsystem::GetVersion() const
{
  if (UBaseGameInstance* gameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    return gameInstance->m_version.ToString();
  }
  return FString();
}
