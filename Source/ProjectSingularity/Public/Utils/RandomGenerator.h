/************************************************************************
 * @description: Will generate a random number
 * @author: Josephine Esposito
 * @date: 25/03/2025
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"

/**
 * Will generate a random number
 */
class PROJECTSINGULARITY_API RandomGenerator
{
private:
  uint64_t m_seed;

public:
  /**
   * @brief The base constructor of the class
   */
  RandomGenerator();

  /**
   * @brief Constructor with a given seed
   * @param _seed 
   */
  RandomGenerator(uint64_t _seed);

  /**
   * @brief Will return a random int between a given range
   * @param _min Default value is 0
   * @param _max Default value is 1000
   * @return 
   */
  int GetRandomIntInRange(int _min = 0, int _max = 1000);

  /**
   * @brief Will return a random float between a given range
   * @param _min Default value is 0
   * @param _max Default value is 1000
   * @return 
   */
  float GetRandomFloatInRange(float _min = 0.0f, float _max = 1000.0f);

  uint64_t GetRNGSeed() const;

  private:
  /**
   * @brief The algoritm that will generate randomness
   * @param _seed The seed to reference
   * @return 
   */
  static uint64_t LCG(uint64_t& _seed);
};
