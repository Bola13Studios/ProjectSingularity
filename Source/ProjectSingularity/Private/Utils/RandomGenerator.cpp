#include "Utils/RandomGenerator.h"
#include "HAL/PlatformTime.h"

RandomGenerator::RandomGenerator()
{ // the seed is time based
  m_seed = static_cast<uint64_t>(FPlatformTime::Cycles64());
}

RandomGenerator::RandomGenerator(uint64_t _seed)
{
  m_seed = _seed == 0 ? 1 : _seed;
}

int RandomGenerator::GetRandomIntInRange(int _min, int _max)
{
  if (_min > _max) Swap(_min, _max);

  uint64_t randValue = LCG(m_seed);

  return _min + (randValue % (_max - _min + 1));
}

float RandomGenerator::GetRandomFloatInRange(float _min, float _max)
{
  if (_min > _max) Swap(_min, _max);

  uint64_t randValue = LCG(m_seed);

  // normalizing between 0 and 1
  float normalized = (randValue / static_cast<float>(UINT64_MAX));

  return _min + normalized * (_max - _min);
}

uint64_t RandomGenerator::GetRNGSeed() const
{
  return m_seed;
}

uint64_t RandomGenerator::LCG(uint64_t& _seed)
{
  const uint64_t a = 6364136223846793005ULL;
  const uint64_t c = 1442695040888963407ULL;

  _seed = a * _seed + c;
  return _seed;
}
