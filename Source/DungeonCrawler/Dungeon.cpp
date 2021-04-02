// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon.h"

#include <vector>

ADungeon::ADungeon()
{
	TileSize = 200.0f;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
}

void ADungeon::AddMesh(UStaticMesh* Mesh, int X, int Y, float Rotation)
{
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), "CornerConcave"));
	MeshComponent->SetStaticMesh(Mesh);
	MeshComponent->SetRelativeLocation(FVector(X*TileSize, Y*TileSize, 0.0f));
	MeshComponent->SetRelativeRotation(FRotator(0.0f, Rotation, 0.0f));
	MeshComponent->SetupAttachment(RootComponent);
}

void ADungeon::Test1()
{
	std::vector<std::vector<char>> Floorplan = {
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' , 'x' },
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' , 'x' },
		{'x', 'x', ' ', ' ', 'x', ' ', ' ', 'x' , 'x' },
		{'x', 'x', ' ', ' ', ' ', ' ', ' ', 'x' , 'x' },
		{'x', 'x', 'x', ' ', ' ', ' ', 'x', 'x' , 'x' },
		{'x', 'x', ' ', ' ', ' ', ' ', ' ', 'x' , 'x' },
		{'x', 'x', ' ', ' ', 'x', ' ', ' ', 'x' , 'x' },
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' , 'x' },
		{'x', 'x', 'x', 'x', 'x', 'x', 'x', 'x' , 'x' },
	};

	for(int Y = 1; Y < Floorplan.size(); ++Y)
	{
		const auto& Above = Floorplan.at(Y-1); 
		const auto& Current = Floorplan.at(Y); 
		for(int X = 1; X < Current.size(); ++X)
		{
			char TileType = Current.at(X);
			char TileTypeLeft = Current.at(X-1);
			char TileTypeAbove = Above.at(X);
			char TileTypeAboveLeft = Above.at(X-1);

			if (TileTypeAboveLeft == 'x' && TileTypeAbove == 'x' && TileTypeLeft == 'x' && TileType == ' ')
			{
				AddMesh(CornerConcaveMesh, X, Y, 180.0f);
			}
			if (TileTypeAboveLeft == 'x' && TileTypeAbove == 'x' && TileTypeLeft == ' ' && TileType == 'x')
			{
				AddMesh(CornerConcaveMesh, X, Y, 270.0f);
			}
			if (TileTypeAboveLeft == 'x' && TileTypeAbove == ' ' && TileTypeLeft == 'x' && TileType == 'x')
			{
				AddMesh(CornerConcaveMesh, X, Y, 90.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == 'x' && TileTypeLeft == 'x' && TileType == 'x')
			{
				AddMesh(CornerConcaveMesh, X, Y, 0.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == ' ' && TileTypeLeft == ' ' && TileType == 'x')
			{
				AddMesh(CornerConvexMesh, X, Y, 270.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == ' ' && TileTypeLeft == 'x' && TileType == ' ')
			{
				AddMesh(CornerConvexMesh, X, Y, 0.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == 'x' && TileTypeLeft == ' ' && TileType == ' ')
			{
				AddMesh(CornerConvexMesh, X, Y, 180.0f);
			}
			if (TileTypeAboveLeft == 'x' && TileTypeAbove == ' ' && TileTypeLeft == ' ' && TileType == ' ')
			{
				AddMesh(CornerConvexMesh, X, Y, 90.0f);
			}
			if (TileTypeAboveLeft == 'x' && TileTypeAbove == 'x' && TileTypeLeft == ' ' && TileType == ' ')
			{
				AddMesh(WallMesh, X, Y, 180.0f);
			}
			if (TileTypeAboveLeft == 'x' && TileTypeAbove == ' ' && TileTypeLeft == 'x' && TileType == ' ')
			{
				AddMesh(WallMesh, X, Y, 90.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == 'x' && TileTypeLeft == ' ' && TileType == 'x')
			{
				AddMesh(WallMesh, X, Y, 270.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == ' ' && TileTypeLeft == 'x' && TileType == 'x')
			{
				AddMesh(WallMesh, X, Y, 0.0f);
			}
			if (TileTypeAboveLeft == ' ' && TileTypeAbove == ' ' && TileTypeLeft == ' ' && TileType == ' ')
			{
				AddMesh(FloorMesh, X, Y, 0.0f);
			}
			if (TileTypeAboveLeft == 'x' && TileTypeAbove == 'x' && TileTypeLeft == 'x' && TileType == 'x')
			{
				AddMesh(CeilingMesh, X, Y, 0.0f);
			}
		}
	}
}

