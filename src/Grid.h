#pragma once
#include "Shader.h"
#include "Mesh.h"

/// <summary>
/// saves the values of the grid like cell size and the overall grid size
/// </summary>
class Grid
{
public:
	int cellSize = 200;
	int gridSize = 45;

public:
	/// <summary>
	/// draws the grid
	/// </summary>
	/// <param name="mesh"> used to render the grid lines</param>
	/// <param name="shader">used to set the transforms of the lines</param>
	/// <param name="showGrid">if it should be drawn</param>
	void Draw(Mesh& mesh, Shader& shader, bool& showGrid, float lineSize);
};