// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include <array>

#include "GameFramework/Actor.h"
#include "Dungeon.generated.h"


UCLASS()
class DUNGEONCRAWLER_API ADungeon : public AActor
{
	GENERATED_BODY()
	
public:	
	ADungeon();

	void AddMesh(UInstancedStaticMeshComponent* Mesh, int X, int Y, float Rotation);
	void AddCellLabel(int X, int Y);

	UFUNCTION(BlueprintCallable)
	void GenerateMaze(int Width, int Height);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* CeilingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* CornerConvexMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInstancedStaticMeshComponent* CornerConcaveMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;
};
