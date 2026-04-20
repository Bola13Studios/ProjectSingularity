// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/VoxelNav/VoxelNavChunk.h"
#include "Systems/VoxelNavigationSubsystem.h"

AVoxelNavChunk::AVoxelNavChunk()
{
  PrimaryActorTick.bCanEverTick = true;
}

// void AVoxelNavChunk::BeginPlay()
//{
//   Super::BeginPlay();
// }

void AVoxelNavChunk::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void AVoxelNavChunk::DrawGrid()
{
  UWorld* world = GetWorld();
  if (!world)
  {
    return;
  }

  UVoxelNavigationSubsystem* nav = world->GetSubsystem<UVoxelNavigationSubsystem>();
  if (!nav)
  {
    return;
  }

  FVector chunkOrigin = GetGridOrigin();

  FIntVector chunkCoord = nav->GetChunkCoord(chunkOrigin);
  FVoxelGridChunk* chunk = nav->GetOrCreateChunk(chunkCoord);

  if (!chunk)
  {
    return;
  }

  float V = chunk->grid.voxelSize;

  int SX = chunk->grid.sizeX;
  int SY = chunk->grid.sizeY;
  int SZ = chunk->grid.sizeZ;

  float duration = -1.f;
  float thickness = 3;
  FColor gridColor = FColor(0, 0, 255, 150);

  // Debug lines
#pragma region DebugLines
  // for (int y = 0; y <= SY; y++)
  //{
  //  for (int z = 0; z <= SZ; z++)
  //  {
  //    FVector start = chunkOrigin + FVector(0, y * V, z * V);
  //    FVector end = chunkOrigin + FVector(SX * V, y * V, z * V);

  //    DrawDebugLine(world, start, end, gridColor, true, duration, 0, thickness);
  //  }
  //}

  // for (int x = 0; x <= SX; x++)
  //{
  //   for (int z = 0; z <= SZ; z++)
  //   {
  //     FVector start = chunkOrigin + FVector(x * V, 0, z * V);
  //     FVector end = chunkOrigin + FVector(x * V, SY * V, z * V);

  //    DrawDebugLine(world, start, end, gridColor, true, duration, 0, thickness);
  //  }
  //}

  // for (int x = 0; x <= SX; x++)
  //{
  //   for (int y = 0; y <= SY; y++)
  //   {
  //     FVector start = chunkOrigin + FVector(x * V, y * V, 0);
  //     FVector end = chunkOrigin + FVector(x * V, y * V, SZ * V);

  //    DrawDebugLine(world, start, end, gridColor, true, duration, 0, thickness);
  //  }
  //}
#pragma endregion

// Debug walkable or blocked
#pragma region DebugWalkableOrBlocked
  for (int x = 0; x < SX; x++)
  {
    for (int y = 0; y < SY; y++)
    {
      for (int z = 0; z < SZ; z++)
      {
        const bool bWalkable = chunk->grid.Get(x, y, z).isWalkable;

        FVector center = chunk->worldOrigin + FVector((x + 0.5f) * V, (y + 0.5f) * V, (z + 0.5f) * V);
        if (!bWalkable)
        {
          DrawDebugPoint(world, center, 5.f, FColor::Red, true, duration);
        }
        else
        {
          DrawDebugPoint(world, center, 5.f, FColor::Green, true, duration);
        }
      }
    }
  }
#pragma endregion
}

void AVoxelNavChunk::DrawChunkGrid()
{
  UWorld* world = GetWorld();
  if (!world) return;

  FlushPersistentDebugLines(world);

  m_drawGrid = !m_drawGrid;
  if (m_drawGrid)
  {
    DrawGrid();
  }
}

void AVoxelNavChunk::DrawPath()
{
  UWorld* world = GetWorld();
  if (!world)
  {
    return;
  }

  UVoxelNavigationSubsystem* nav = world->GetSubsystem<UVoxelNavigationSubsystem>();
  if (!nav)
  {
    return;
  }

  FVector startWorld = startPoint.GetLocation();
  FVector endWorld = endPoint.GetLocation();

  FVoxelGridChunk* chunk = nav->GetOrCreateChunk(nav->GetChunkCoord(startWorld));
  if (!chunk)
  {
    return;
  }

  TArray<FVector> path = nav->FindPath(startWorld, endWorld, GetGridOrigin());

  if (path.Num() == 0)
  {
    return;
  }

  for (int i = 0; i < path.Num() - 1; i++)
  {
    DrawDebugLine(world, path[i], path[i + 1], FColor::Yellow, false, 10.f, 0, 3.f);
  }

  path = nav->OptimizePath(path, world);

  for (int i = 0; i < path.Num() - 1; i++)
  {
    DrawDebugLine(world, path[i], path[i + 1], FColor::Red, false, 10.f, 0, 3.f);
  }

  DrawDebugSphere(world, startWorld, 25.f, 12, FColor::Green, false, 10.f);
  DrawDebugSphere(world, endWorld, 25.f, 12, FColor::Red, false, 10.f);
}

FVector AVoxelNavChunk::GetGridOrigin() const
{
  return GetActorLocation();
}
