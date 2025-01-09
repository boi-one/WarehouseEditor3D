#include "Mesh.h"

void Mesh::CreateCube()
{
	GLfloat vertices[] =
	{
		//voor (maar omdat nu alles is gedraaid boven en onder)
		 1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//0
		 1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//1
		-1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//2
		-1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//3
		//achter	   (onder) 
		 1.0f,  1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//4
		 1.0f, -1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//5
		-1.0f, -1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//6
		-1.0f,  1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//7
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,

		3, 2, 6,
		3, 6, 7,

		7, 6, 5,
		7, 5, 4,

		4, 5, 1,
		4, 1, 0,

		5, 1, 6,
		1, 2, 6,

		4, 0, 7,
		0, 3, 7,
	};

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

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	verticesCount = sizeof(indices) / sizeof(GLuint);
}

void Mesh::Draw(Shader& shader)
{
	shader.use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
