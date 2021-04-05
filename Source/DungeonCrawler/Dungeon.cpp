// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon.h"

#include <vector>

#include "Components/TextRenderComponent.h"

ADungeon::ADungeon()
{
	TileSize = 200.0f;
	RootComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Root"));
}

void ADungeon::AddMesh(UStaticMesh* Mesh, int X, int Y, float Rotation)
{
	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass(), MakeUniqueObjectName(this, UStaticMeshComponent::StaticClass(), "CornerConcave"));
	MeshComponent->SetStaticMesh(Mesh);
	MeshComponent->SetRelativeLocation(FVector(X, Y, 0.0f));
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

}

enum Direction
{
	North,
    East,
    South,
    West,
	NumDirections
};

struct FCell
{
	bool Walls[4] = {true, true, true, true};
	bool Visited = false;
};

class FMazeGenerator
{
public:
	FMazeGenerator(int W, int H, int32 Seed) : Width(W), Height(H), RNG(Seed)
	{
		Cells = new FCell[Width*Height];
	}

	~FMazeGenerator()
	{
		delete [] Cells;
	}

	FCell* GetCell(int X, int Y)
	{
		if (X < 0 || X >= Width)
		{
			return nullptr;
		}
		if (Y < 0 || Y >= Height)
		{
			return nullptr;
		}
		return &Cells[X + Y*Width];
	}

	FCell* GetNeighbor(int X, int Y, Direction Dir)
	{
		switch(Dir)
		{
			case North:
				return GetCell(X, Y-1);
			case East:
				return GetCell(X+1, Y);
			case South:
				return GetCell(X, Y+1);
			case West:
				return GetCell(X-1, Y);
			default:
				return nullptr;
		}
	}
	
	void GenerateRecursive(int X, int Y)
	{
		FCell* Cell = GetCell(X, Y);
		Cell->Visited = true;
		UE_LOG(LogTemp, Display, TEXT("Visiting %d, %d"), X, Y);

		while (true)
		{
			UE_LOG(LogTemp, Display, TEXT("Counting neighbors for %d, %d"), X, Y);
			int Neighbors[4];
			int NumNeighbors = 0;
			for (int Dir = 0; Dir < NumDirections; Dir++)
			{
				FCell* Neighbor = GetNeighbor(X, Y, static_cast<Direction>(Dir));
				if (Neighbor && !Neighbor->Visited)
				{
					Neighbors[NumNeighbors] = Dir;
					NumNeighbors++;
				}
			}
			UE_LOG(LogTemp, Display, TEXT("Num neighbors: %d"), NumNeighbors);

			if (NumNeighbors == 0)
			{
				break;
			}
			int NeighborIndex = 0;
			if (NumNeighbors > 1)
			{
				NeighborIndex = RNG.RandRange(0, NumNeighbors - 1);
			}
			Direction Dir = static_cast<Direction>(Neighbors[NeighborIndex]);
			FCell* Neighbor = GetNeighbor(X, Y, Dir);
			switch(Dir)
			{
			case North:
				Cell->Walls[North] = false;
				Neighbor->Walls[South] = false;
				GenerateRecursive(X, Y-1);
				break;
			case East:
				Cell->Walls[East] = false;
				Neighbor->Walls[West] = false;
				GenerateRecursive(X+1, Y);
				break;
			case South:
				Cell->Walls[South] = false;
				Neighbor->Walls[North] = false;
				GenerateRecursive(X, Y+1);
				break;
			case West:
				Cell->Walls[West] = false;
				Neighbor->Walls[East] = false;
				GenerateRecursive(X-1, Y);
				break;
			default:
				break;
			}
		}
		UE_LOG(LogTemp, Display, TEXT("Done with %d, %d"), X, Y);
	}

protected:
	FCell* Cells;
	int Width;
	int Height;
	FRandomStream RNG;
};

void ADungeon::GenerateMaze(int Width, int Height)
{
	FMazeGenerator M(Width, Height, 123);
	M.GenerateRecursive(1, 1);

	std::vector<std::vector<char>> Floorplan;
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

			FText Text = FText::FromString(FString::Printf(TEXT("(%d,%d)"), X, Y));
			UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this, UTextRenderComponent::StaticClass(), MakeUniqueObjectName(this, UTextRenderComponent::StaticClass(), "Text"));
			TextComponent->SetText(Text);
			TextComponent->SetRelativeLocation(FVector(XPos, YPos, 10.0f));
			TextComponent->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f));
			TextComponent->SetupAttachment(RootComponent);

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

