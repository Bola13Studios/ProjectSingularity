#include "Systems/VoxelNavigationSubsystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CoreMinimal.h"
#include "Systems/VoxelNavigationSubsystem.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Algo/Reverse.h"

FIntVector UVoxelNavigationSubsystem::GetChunkCoord(const FVector& WorldPos) const
{
  return FIntVector(FMath::FloorToInt(WorldPos.X / chunkWorldSize), FMath::FloorToInt(WorldPos.Y / chunkWorldSize),
                    FMath::FloorToInt(WorldPos.Z / chunkWorldSize));
}

FVoxelGridChunk* UVoxelNavigationSubsystem::GetOrCreateChunk(const FIntVector& Coord)
{
  if (FVoxelGridChunk* found = gridChunks.Find(Coord)) return found;

  FVoxelGridChunk& chunk = gridChunks.Add(Coord);
  chunk.chunkCoord = Coord;

  chunk.worldOrigin = FVector(Coord.X * chunkWorldSize, Coord.Y * chunkWorldSize, Coord.Z * chunkWorldSize);

  chunk.grid.Init(64, 64, 32, 100.f); // Temp - Need to expose this to editor

  BuildChunk(chunk);

  return &chunk;
}

void UVoxelNavigationSubsystem::BuildChunk(FVoxelGridChunk& Chunk)
{
  UWorld* world = GetWorld();
  if (!world) return;

  const float radius = Chunk.grid.voxelSize * 0.45f;
  const float halfHeight = Chunk.grid.voxelSize * 0.5f;

  for (int x = 0; x < Chunk.grid.sizeX; x++)
  {
    for (int y = 0; y < Chunk.grid.sizeY; y++)
    {
      for (int z = 0; z < Chunk.grid.sizeZ; z++)
      {
        FVector center = GridToWorldInChunk(Chunk, FIntVector(x, y, z));

        FCollisionShape shape = FCollisionShape::MakeCapsule(radius, halfHeight);

        FCollisionQueryParams params;
        params.bTraceComplex = false;

        bool blocked = world->OverlapBlockingTestByChannel(center, FQuat::Identity, ECC_Visibility, shape, params);
        Chunk.grid.Get(x, y, z).isWalkable = !blocked;
      }
    }
  }
}

TArray<FIntVector> UVoxelNavigationSubsystem::GetNeighbors(const FIntVector& P)
{
  return {P + FIntVector(1, 0, 0),  P + FIntVector(-1, 0, 0), P + FIntVector(0, 1, 0),
          P + FIntVector(0, -1, 0), P + FIntVector(0, 0, 1),  P + FIntVector(0, 0, -1)};
}

float UVoxelNavigationSubsystem::Heuristic(const FIntVector& A, const FIntVector& B) const
{
  FIntVector D = A - B;
  return FMath::Abs(D.X) + FMath::Abs(D.Y) + FMath::Abs(D.Z);
}

FVector UVoxelNavigationSubsystem::GridToWorldInChunk(const FVoxelGridChunk& Chunk, const FIntVector& L) const
{
  return Chunk.worldOrigin
      + FVector((L.X + 0.5f) * Chunk.grid.voxelSize, (L.Y + 0.5f) * Chunk.grid.voxelSize,
                (L.Z + 0.5f) * Chunk.grid.voxelSize);
}

FIntVector UVoxelNavigationSubsystem::WorldToLocalVoxel(const FVector& World, const FVoxelGridChunk& Chunk) const
{
  FVector local = (World - Chunk.worldOrigin) / Chunk.grid.voxelSize;

  return FIntVector(FMath::FloorToInt(local.X), FMath::FloorToInt(local.Y), FMath::FloorToInt(local.Z));
}

FIntVector UVoxelNavigationSubsystem::FindNearestWalkable(const FVoxelGrid& Grid, const FIntVector& From)
{
  if (Grid.IsValid(From.X, From.Y, From.Z) && Grid.Get(From.X, From.Y, From.Z).isWalkable)
  {
    return From;
  }

  TQueue<FIntVector> queue;
  TSet<FIntVector> visited;

  queue.Enqueue(From);
  visited.Add(From);

  const int maxSearch = 1000;
  int count = 0;

  while (!queue.IsEmpty() && count++ < maxSearch)
  {
    FIntVector Current;
    queue.Dequeue(Current);

    for (const FIntVector& N : GetNeighbors(Current))
    {
      if (!Grid.IsValid(N.X, N.Y, N.Z) || visited.Contains(N))
      {
        continue;
      }

      if (Grid.Get(N.X, N.Y, N.Z).isWalkable)
      {
        return N;
      }

      visited.Add(N);
      queue.Enqueue(N);
    }
  }

  return From;
}

TArray<FVector> UVoxelNavigationSubsystem::FindPath(FVector StartWorld, FVector EndWorld, const FVector& Origin)
{
  TArray<FVector> result;

  FVoxelGridChunk* chunk = GetOrCreateChunk(GetChunkCoord(StartWorld));
  if (!chunk) return result;

  FIntVector start = WorldToLocalVoxel(StartWorld, *chunk);
  FIntVector goal = WorldToLocalVoxel(EndWorld, *chunk);

  start = FindNearestWalkable(chunk->grid, start);
  goal = FindNearestWalkable(chunk->grid, goal);

  TArray<FAStarNode> open;
  TSet<FIntVector> closed;
  TMap<FIntVector, FIntVector> cameFrom;
  TMap<FIntVector, float> bestG;

  FAStarNode startNode;
  startNode.pos = start;
  startNode.G = 0;
  startNode.H = Heuristic(start, goal);

  open.Add(startNode);
  bestG.Add(start, 0);

  int safety = 0;

  while (open.Num() > 0)
  {
    if (++safety > 20000) break;

    int bestIndex = 0;
    for (int i = 1; i < open.Num(); i++)
      if (open[i].F() < open[bestIndex].F()) bestIndex = i;

    FAStarNode current = open[bestIndex];
    open.RemoveAtSwap(bestIndex);

    if (current.pos == goal)
    {
      TArray<FVector> path;

      FIntVector step = goal;
      path.Add(GridToWorldInChunk(*chunk, step));

      while (step != start)
      {
        step = cameFrom[step];
        path.Add(GridToWorldInChunk(*chunk, step));
      }

      Algo::Reverse(path);

      if (path.Num() > 0)
      {
        path[0] = StartWorld;
        path.Last() = EndWorld;
      }
      return path;
    }

    closed.Add(current.pos);

    for (const FIntVector& N : GetNeighbors(current.pos))
    {
      if (!chunk->grid.IsValid(N.X, N.Y, N.Z)) continue;
      if (!chunk->grid.Get(N.X, N.Y, N.Z).isWalkable) continue;
      if (closed.Contains(N)) continue;

      float NewG = current.G + 1.f;

      if (bestG.Contains(N) && bestG[N] <= NewG) continue;

      bestG.Add(N, NewG);
      cameFrom.FindOrAdd(N) = current.pos;
      FAStarNode Neighbor;
      Neighbor.pos = N;
      Neighbor.G = NewG;
      Neighbor.H = Heuristic(N, goal);

      open.Add(Neighbor);
    }
  }

  result.Add(StartWorld);
  result.Add(EndWorld);
  return result;
}
TArray<FVector> UVoxelNavigationSubsystem::OptimizePath(const TArray<FVector>& InPath, UWorld* World)
{
  TArray<FVector> result;

  if (InPath.Num() == 0)
  {
    return result;
  }

  int32 current = 0;
  result.Add(InPath[0]);

  while (current < InPath.Num() - 1)
  {
    int32 best = current + 1;

    for (int i = current + 2; i < InPath.Num(); i++)
    {
      FHitResult Hit;
      bool blocked = World->SweepSingleByChannel(Hit, InPath[current], InPath[i], FQuat::Identity, ECC_Visibility,
                                                 FCollisionShape::MakeSphere(50.f));
      if (!blocked)
      {
        best = i;
      }
      else
      {
        break;
      }
    }

    current = best;
    result.Add(InPath[current]);
  }

  return result;
}

bool UVoxelNavigationSubsystem::IsWorldOccupied(const FVector& WorldPos, const FVector& Origin)
{
  FVoxelGridChunk* chunk = GetOrCreateChunk(GetChunkCoord(WorldPos));
  if (!chunk)
  {
    return true;
  }

  FIntVector Local = WorldToLocalVoxel(WorldPos, *chunk);

  return chunk->grid.IsOccupied(Local.X, Local.Y, Local.Z);
}
