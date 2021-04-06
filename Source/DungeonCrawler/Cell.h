#pragma once

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

