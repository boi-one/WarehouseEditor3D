#include "Mesh.h"

void Mesh::Draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}