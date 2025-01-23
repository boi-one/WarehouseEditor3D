#include "Grid.h"

void Grid::Draw(Mesh& mesh, Shader& shader, bool& showGrid, float lineSize)
{
	if (!showGrid) return;

	shader.setVec3("mColor", { 07.f, 0.7f, 0.7f });
	for (int i = -gridSize; i < gridSize; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { i * cellSize , 0, 0 });
		model = glm::scale(model, { lineSize, gridSize * cellSize, lineSize });
		shader.setMat4("model", model);
		mesh.Draw(shader);
	}
	for (int i = -gridSize; i < gridSize; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { 0, i * cellSize, 0 });
		model = glm::scale(model, { gridSize * cellSize, lineSize, lineSize });
		shader.setMat4("model", model);
		mesh.Draw(shader);
	}
}