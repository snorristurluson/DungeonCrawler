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

	void AddMesh(UStaticMesh* Mesh, int X, int Y, float Rotation);
	
	UFUNCTION(BlueprintCallable)
	void Test1();

	UFUNCTION(BlueprintCallable)
	void GenerateMaze(int Width, int Height);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* FloorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* CeilingMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* WallMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* CornerConvexMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* CornerConcaveMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TileSize;
};
