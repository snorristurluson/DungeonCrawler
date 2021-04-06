#include "MazeGenerator.h"

FMazeGenerator::FMazeGenerator(int W, int H, int32 Seed): Width(W), Height(H), RNG(Seed)
{
	Cells = new FCell[Width * Height];
}

FMazeGenerator::~FMazeGenerator()
{
	delete [] Cells;
}

FCell* FMazeGenerator::GetCell(int X, int Y) const
{
	if (X < 0 || X >= Width)
	{
		return nullptr;
	}
	if (Y < 0 || Y >= Height)
	{
		return nullptr;
	}
	return &Cells[X + Y * Width];
}

FCell* FMazeGenerator::GetNeighbor(int X, int Y, Direction Dir) const
{
	switch (Dir)
	{
	case North:
		return GetCell(X, Y - 1);
	case East:
		return GetCell(X + 1, Y);
	case South:
		return GetCell(X, Y + 1);
	case West:
		return GetCell(X - 1, Y);
	default:
		return nullptr;
	}
}

int FMazeGenerator::GetNonVisitedNeighbors(int X, int Y, int Neighbors[4]) const
{
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
	return NumNeighbors;
}

void FMazeGenerator::GenerateRecursive(int X, int Y)
{
	FCell* Cell = GetCell(X, Y);
	Cell->Visited = true;
	UE_LOG(LogTemp, Display, TEXT("Visiting %d, %d"), X, Y);

	while (true)
	{
		UE_LOG(LogTemp, Display, TEXT("Counting neighbors for %d, %d"), X, Y);
		int Neighbors[4];
		int NumNeighbors = GetNonVisitedNeighbors(X, Y, Neighbors);
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
		const Direction Dir = static_cast<Direction>(Neighbors[NeighborIndex]);
		FCell* Neighbor = GetNeighbor(X, Y, Dir);
		switch (Dir)
		{
		case North:
			Cell->Walls[North] = false;
			Neighbor->Walls[South] = false;
			GenerateRecursive(X, Y - 1);
			break;
		case East:
			Cell->Walls[East] = false;
			Neighbor->Walls[West] = false;
			GenerateRecursive(X + 1, Y);
			break;
		case South:
			Cell->Walls[South] = false;
			Neighbor->Walls[North] = false;
			GenerateRecursive(X, Y + 1);
			break;
		case West:
			Cell->Walls[West] = false;
			Neighbor->Walls[East] = false;
			GenerateRecursive(X - 1, Y);
			break;
		default:
			break;
		}
	}
	UE_LOG(LogTemp, Display, TEXT("Done with %d, %d"), X, Y);
}
