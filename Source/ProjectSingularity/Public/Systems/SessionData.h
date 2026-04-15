/************************************************************************
 * @description: Holds the base structure of the statistics to save
 * @author: Josephine Esposito
 * @date: 24/03/2026
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "SessionData.generated.h"

#pragma region | Combat Stats |

USTRUCT(BlueprintType)
struct FCombatUpgrades
{
  GENERATED_BODY()

  UPROPERTY()
  TArray<FString> combat_start_upgrades;
  UPROPERTY()
  TArray<FString> combat_end_upgrades;
};

USTRUCT(BlueprintType)
struct FCombatDamage
{
  GENERATED_BODY()

  UPROPERTY()
  int32 combat_total_damage = 0;
  UPROPERTY()
  int32 combat_total_damage_short_range = 0;
  UPROPERTY()
  int32 combat_total_damage_long_range = 0;
};

USTRUCT(BlueprintType)
struct FCombatShots
{
  GENERATED_BODY()

  UPROPERTY()
  int32 combat_total_shots = 0;
  UPROPERTY()
  int32 combat_total_shots_short_range = 0;
  UPROPERTY()
  int32 combat_total_shots_long_range = 0;
};

USTRUCT(BlueprintType)
struct FCombatHits
{
  GENERATED_BODY()
  UPROPERTY()
  int32 combat_total_hits = 0;
  UPROPERTY()
  int32 combat_total_hits_short_range = 0;
  UPROPERTY()
  int32 combat_total_hits_long_range = 0;
};

USTRUCT(BlueprintType)
struct FCombatCrits
{
  GENERATED_BODY()
  UPROPERTY()
  int32 combat_total_critical_hits = 0;
  UPROPERTY()
  int32 combat_total_critical_hits_short_range = 0;
  UPROPERTY()
  int32 combat_total_critical_hits_long_range = 0;
};

USTRUCT(BlueprintType)
struct FCombatHealth
{
  GENERATED_BODY()
  UPROPERTY()
  int32 combat_total_lost_health = 0;
  UPROPERTY()
  int32 combat_total_gained_health = 0;
  UPROPERTY()
  int32 combat_health_bought = 0;
  UPROPERTY()
  int32 combat_health_won = 0;
};

USTRUCT(BlueprintType)
struct FCombatAmmo
{
  GENERATED_BODY()

  UPROPERTY()
  int32 combat_total_used_ammo = 0;
  UPROPERTY()
  int32 combat_total_gained_ammo = 0;
  UPROPERTY()
  int32 combat_total_bought_ammo = 0;
  UPROPERTY()
  int32 combat_total_won_ammo = 0;

  UPROPERTY()
  int32 combat_total_used_ammo_short_range = 0;
  UPROPERTY()
  int32 combat_total_used_ammo_long_range = 0;
};

USTRUCT(BlueprintType)
struct FCombatStats
{
  GENERATED_BODY()

  UPROPERTY()
  FCombatUpgrades upgrades;
  UPROPERTY()
  FCombatDamage damage;
  UPROPERTY()
  FCombatShots shots;
  UPROPERTY()
  FCombatHits hits;
  UPROPERTY()
  FCombatCrits crits;
  UPROPERTY()
  FCombatHealth health;
  UPROPERTY()
  int32 combat_total_deaths = 0;
  UPROPERTY()
  FCombatAmmo ammo;
};

#pragma endregion

#pragma region | Hype Stats |

USTRUCT(BlueprintType)
struct FHypeStats
{
  GENERATED_BODY()

  UPROPERTY()
  int32 total_gained_hype = 0;
  UPROPERTY()
  int32 total_gained_hype_short_range = 0;
  UPROPERTY()
  int32 total_gained_hype_long_range = 0;
  UPROPERTY()
  int32 total_gained_hype_normal_hit = 0;
  UPROPERTY()
  int32 total_gained_hype_critical_hit = 0;
  UPROPERTY()
  int32 total_gained_hype_multikill = 0;
  UPROPERTY()
  int32 total_used_hype_onhealth = 0;
  UPROPERTY()
  int32 total_used_hype_onammo = 0;
  UPROPERTY()
  int32 total_used_hype_onupgrades = 0;
};

#pragma endregion

#pragma region | Slot Machine Stats |

USTRUCT(BlueprintType)
struct FSlotMachineStats
{
  GENERATED_BODY()
  UPROPERTY()
  int32 slotmachine_total_uses = 0;
  UPROPERTY()
  int32 slotmachine_total_wins = 0;
  UPROPERTY()
  int32 slotmachine_total_positive_wins = 0;
  UPROPERTY()
  int32 slotmachine_total_negative_wins = 0;
  UPROPERTY()
  int32 slotmachine_total_health_wins = 0;
  UPROPERTY()
  int32 slotmachine_total_ammo_wins = 0;
  UPROPERTY()
  int32 slotmachine_total_other_wins = 0;
  UPROPERTY()
  TArray<FString> slotmachine_start_symbols = {};
  UPROPERTY()
  TArray<FString> slotmachine_end_symbols = {};
};

#pragma endregion

#pragma region | Enemy Stats |

USTRUCT(BlueprintType)
struct FEnemyTypeStats
{
  GENERATED_BODY()

  UPROPERTY()
  int32 enemy_total_deaths = 0;
  UPROPERTY()
  int32 enemy_total_critical_hits = 0;
  UPROPERTY()
  int32 enemy_total_critical_deaths = 0;
  UPROPERTY()
  int32 enemy_total_hits = 0;
};

USTRUCT(BlueprintType)
struct FEnemy
{
  GENERATED_BODY()

  UPROPERTY()
  FEnemyTypeStats chaser;
  UPROPERTY()
  FEnemyTypeStats dron;
};

#pragma endregion

/**
 * Holds the base structure of the statistics to save
 */
USTRUCT(BlueprintType)
struct FSessionData
{
  GENERATED_BODY()

  UPROPERTY()
  FName playerName;
  UPROPERTY()
  FName version;

  UPROPERTY()
  FCombatStats combat;
  UPROPERTY()
  FHypeStats hype;
  UPROPERTY()
  FSlotMachineStats slotmachine;
  UPROPERTY()
  FEnemy enemy;
};
