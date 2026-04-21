// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VoxelNavChunk.generated.h"

UCLASS()
class PROJECTSINGULARITY_API AVoxelNavChunk : public AActor
{
  GENERATED_BODY()

public:
  AVoxelNavChunk();
  virtual void Tick(float DeltaTime) override;

  void DrawGrid();
  UFUNCTION(CallInEditor)
  void DrawChunkGrid();
  UFUNCTION(CallInEditor)
  void DrawPath();

  FVector GetGridOrigin() const;

public:
  UPROPERTY(EditAnywhere)
  bool drawDebug = true;

  UPROPERTY(EditAnywhere)
  float debugDuration = 10.f;

  UPROPERTY(EditAnywhere, Category = "Path Debug", meta = (MakeEditWidget = true))
  FTransform startPoint;

  UPROPERTY(EditAnywhere, Category = "Path Debug", meta = (MakeEditWidget = true))
  FTransform endPoint;
private:
  bool m_drawGrid = false;
};
