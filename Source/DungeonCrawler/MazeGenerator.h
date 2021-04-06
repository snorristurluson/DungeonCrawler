#pragma once
#include "Cell.h"

class FMazeGenerator
{
public:
	FMazeGenerator(int W, int H, int32 Seed);
	~FMazeGenerator();

	FCell* GetCell(int X, int Y) const;
	FCell* GetNeighbor(int X, int Y, Direction Dir) const;

	void GenerateRecursive(int X, int Y);

protected:
	FCell* Cells;
	int Width;
	int Height;
	FRandomStream RNG;

	int GetNonVisitedNeighbors(int X, int Y, int Neighbors[4]) const;
};

