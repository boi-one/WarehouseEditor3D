#pragma once
#include "Shader.h"
#include "Mesh.h"

class Grid
{
public:
	int cellSize = 200;
	int gridSize = 45;

public:
	void Draw(Mesh& mesh, Shader& shader, bool& showGrid);
};