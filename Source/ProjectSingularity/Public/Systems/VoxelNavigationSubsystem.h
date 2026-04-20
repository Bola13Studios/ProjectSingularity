#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VoxelNavigationSubsystem.generated.h"

struct FGridNode
{
  bool isWalkable = true;
};

USTRUCT()
struct FVoxelGrid
{
  GENERATED_BODY()

public:
  int sizeX = 0;
  int sizeY = 0;
  int sizeZ = 0;

  float voxelSize = 100.f;

  TArray<FGridNode> nodes;

  void Init(int X, int Y, int Z, float Size)
  {
    sizeX = X;
    sizeY = Y;
    sizeZ = Z;
    voxelSize = Size;
    nodes.SetNum(X * Y * Z);
  }

  FORCEINLINE int Index(int X, int Y, int Z) const
  {
    return X + Y * sizeX + Z * sizeX * sizeY;
  }

  FORCEINLINE bool IsValid(int X, int Y, int Z) const
  {
    return X >= 0 && Y >= 0 && Z >= 0 && X < sizeX && Y < sizeY && Z < sizeZ;
  }

  FORCEINLINE FGridNode& Get(int X, int Y, int Z)
  {
    return nodes[Index(X, Y, Z)];
  }

  FORCEINLINE const FGridNode& Get(int X, int Y, int Z) const
  {
    return nodes[Index(X, Y, Z)];
  }

  FORCEINLINE bool IsOccupied(int X, int Y, int Z) const
  {
    if (!IsValid(X, Y, Z)) return true;
    return !nodes[Index(X, Y, Z)].isWalkable;
  }
};

USTRUCT()
struct FVoxelGridChunk
{
  GENERATED_BODY()

public:
  FIntVector chunkCoord;

  FVector worldOrigin;

  FVoxelGrid grid;
};

USTRUCT()
struct FAStarNode
{
  GENERATED_BODY()

  FIntVector pos;
  float G = 0;
  float H = 0;

  float F() const
  {
    return G + H;
  }
};

UCLASS()
class PROJECTSINGULARITY_API UVoxelNavigationSubsystem : public UWorldSubsystem
{
  GENERATED_BODY()

public:
  FIntVector GetChunkCoord(const FVector& WorldPos) const;
  FVoxelGridChunk* GetOrCreateChunk(const FIntVector& Coord);
  FVector GridToWorldInChunk(const FVoxelGridChunk& Chunk, const FIntVector& L) const;

  FIntVector WorldToLocalVoxel(const FVector& World, const FVoxelGridChunk& Chunk) const;
  FIntVector FindNearestWalkable(const FVoxelGrid& Grid, const FIntVector& From);

  TArray<FVector> FindPath(FVector StartWorld, FVector EndWorld, const FVector& Origin = FVector::ZeroVector);
  TArray<FVector> OptimizePath(const TArray<FVector>& InPath, UWorld* World);

  bool IsWorldOccupied(const FVector& WorldPos, const FVector& Origin);

private:
  void BuildChunk(FVoxelGridChunk& Chunk);
  TArray<FIntVector> GetNeighbors(const FIntVector& P);
  float Heuristic(const FIntVector& A, const FIntVector& B) const;

public:
  TMap<FIntVector, FVoxelGridChunk> gridChunks;
  float chunkWorldSize = 5000.f;
};
