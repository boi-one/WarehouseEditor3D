#include "Mesh.h"

void Mesh::Draw(std::vector<GLfloat> vertices, GLuint& EBO, GLuint indices[6])
{
	glBindVertexArray(VAO);

	/*glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * vertices.size(), vertices.data());

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices);*/

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
