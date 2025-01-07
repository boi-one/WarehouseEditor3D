#include "Mesh.h"

void Mesh::CreatePlane()
{
	GLfloat vertices[] =
	{
		//pos				 color
		 1.0f,  1.0f, 1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 1.0f,  0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 1.0f,  1.0f, 1.0f, 1.0f
	};

	GLuint indices[] =
	{
		0, 3, 1,
		3, 1, 2
	};
	GLuint VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0);

	verticesCount = 6;
}

void Mesh::Draw(Shader& shader)
{
	shader.use();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
