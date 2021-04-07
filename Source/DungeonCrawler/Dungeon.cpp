// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon.h"

#include <vector>


#include "Cell.h"
#include "MazeGenerator.h"
#include "Components/TextRenderComponent.h"

ADungeon::ADungeon()
{
	TileSize = 200.0f;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
	FloorMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Floor"));
	CeilingMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Ceiling"));
	WallMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Wall"));
	CornerConvexMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CornerConvex"));
	CornerConcaveMesh = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("CornerConcave"));
}

void ADungeon::AddMesh(UInstancedStaticMeshComponent* Mesh, int X, int Y, float Rotation)
{
	FTransform Transform(FRotator(0.0f, Rotation, 0.0f), FVector(X, Y, 0.0f));
	Mesh->AddInstance(Transform);
}

void ADungeon::AddCellLabel(int X, int Y)
{
	const float XPos = X*3*TileSize;
	const float YPos = Y*3*TileSize;
	const FText Text = FText::FromString(FString::Printf(TEXT("(%d,%d)"), X, Y));
	UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass(), MakeUniqueObjectName(this, UTextRenderComponent::StaticClass(), "Text"));
	TextComponent->SetText(Text);
	TextComponent->SetRelativeLocation(FVector(XPos, YPos, 10.0f));
	TextComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
	TextComponent->SetupAttachment(RootComponent);
}

void ADungeon::GenerateMaze(int Width, int Height)
{
	FloorMesh->ClearInstances();
	CeilingMesh->ClearInstances();
	WallMesh->ClearInstances();
	CornerConvexMesh->ClearInstances();
	CornerConcaveMesh->ClearInstances();
	
	FMazeGenerator M(Width, Height, 123);
	M.GenerateRecursive(1, 1);
	
	for (int Y = 0; Y < Height; Y++)
	{
		const float YPos = Y*3*TileSize;
		for (int X = 0; X < Width; X++)
		{
			FCell* Cell = M.GetCell(X, Y);
			FCell* NorthNeighbor = M.GetNeighbor(X, Y, North);
			FCell* EastNeighbor = M.GetNeighbor(X, Y, East);
			FCell* SouthNeighbor = M.GetNeighbor(X, Y, South);
			FCell* WestNeighbor = M.GetNeighbor(X, Y, West);

			const float XPos = X*3*TileSize;

			AddMesh(FloorMesh, XPos, YPos, 0.0f);
			if (Cell->Walls[North])
			{
				AddMesh(WallMesh, XPos, YPos - TileSize, 180.0f);
				if (Cell->Walls[West])
				{
					AddMesh(CornerConcaveMesh, XPos - TileSize, YPos - TileSize, 180.0f);
				} else
				{
					AddMesh(WallMesh, XPos - TileSize, YPos - TileSize, 180.0f);
				}
				if (Cell->Walls[East])
				{
					AddMesh(CornerConcaveMesh, XPos + TileSize, YPos - TileSize, 270.0f);
				} else
				{
					AddMesh(WallMesh, XPos + TileSize, YPos - TileSize, 180.0f);
				}
			} else
			{
				AddMesh(FloorMesh, XPos, YPos - TileSize, 0.0f);
				if (Cell->Walls[East])
				{
					if (EastNeighbor && EastNeighbor->Walls[North] || NorthNeighbor && NorthNeighbor->Walls[East])
					{
						AddMesh(WallMesh, XPos + TileSize, YPos - TileSize, 270.0f);
					} else
					{
						AddMesh(CornerConvexMesh, XPos + TileSize, YPos - TileSize, 270.0f);
					}
				} else
				{
					if (EastNeighbor && EastNeighbor->Walls[North])
					{
						AddMesh(CornerConvexMesh, XPos + TileSize, YPos - TileSize, 180.0f);
					} else
					{
						AddMesh(FloorMesh, XPos + TileSize, YPos - TileSize, 0.0f);
					}
				}
				if (Cell->Walls[West])
				{
					if (NorthNeighbor && NorthNeighbor->Walls[West] || WestNeighbor && WestNeighbor->Walls[North])
					{
						AddMesh(WallMesh, XPos - TileSize, YPos - TileSize, 90.0f);
					} else
					{
						AddMesh(CornerConvexMesh, XPos - TileSize, YPos - TileSize, 0.0f);
					}
				} else
				{
					if (WestNeighbor && WestNeighbor->Walls[North])
					{
						AddMesh(CornerConvexMesh, XPos - TileSize, YPos - TileSize, 90.0f);
					} else
					{
						AddMesh(FloorMesh, XPos - TileSize, YPos - TileSize, 0.0f);
					}
				}
			}

			if (Cell->Walls[South])
			{
				AddMesh(WallMesh, XPos, YPos + TileSize, 0.0f);
				if (Cell->Walls[West])
				{
					AddMesh(CornerConcaveMesh, XPos - TileSize, YPos + TileSize, 90.0f);
				} else
				{
					AddMesh(WallMesh, XPos - TileSize, YPos + TileSize, 0.0f);
				}
				if (Cell->Walls[East])
				{
					AddMesh(CornerConcaveMesh, XPos + TileSize, YPos + TileSize, 0.0f);
				} else
				{
					AddMesh(WallMesh, XPos + TileSize, YPos + TileSize, 0.0f);
				}
			} else
			{
				AddMesh(FloorMesh, XPos, YPos + TileSize, 0.0f);
				if (Cell->Walls[East])
				{
					if (SouthNeighbor && SouthNeighbor->Walls[East] || EastNeighbor && EastNeighbor->Walls[South])
					{
						AddMesh(WallMesh, XPos + TileSize, YPos + TileSize, 270.0f);
					} else
					{
						AddMesh(CornerConvexMesh, XPos + TileSize, YPos + TileSize, 180.0f);
					}
				}
				else
				{
					if (EastNeighbor && EastNeighbor->Walls[South])
					{
						AddMesh(CornerConvexMesh, XPos + TileSize, YPos + TileSize, 270.0f);
					} else
					{
						AddMesh(FloorMesh, XPos + TileSize, YPos + TileSize, 0.0f);
					}
				}
				if (Cell->Walls[West])
				{
					if (SouthNeighbor && SouthNeighbor->Walls[West] || WestNeighbor && WestNeighbor->Walls[South])
					{
						AddMesh(WallMesh, XPos - TileSize, YPos + TileSize, 90.0f);
					} else
					{
						AddMesh(CornerConvexMesh, XPos - TileSize, YPos + TileSize, 90.0f);
					}
				} else
				{
					if (WestNeighbor && WestNeighbor->Walls[South])
					{
						AddMesh(CornerConvexMesh, XPos - TileSize, YPos + TileSize, 0.0f);
					} else
					{
						AddMesh(FloorMesh, XPos - TileSize, YPos + TileSize, 0.0f);
					}
				}
			}

			if (Cell->Walls[West])
			{
				AddMesh(WallMesh, XPos - TileSize, YPos, 90.0f);
			} else
			{
				AddMesh(FloorMesh, XPos - TileSize, YPos, 0.0f);
			}
			if (Cell->Walls[East])
			{
				AddMesh(WallMesh, XPos + TileSize, YPos, 270.0f);
			} else
			{
				AddMesh(FloorMesh, XPos + TileSize, YPos, 0.0f);
			}
		}
	}
}

