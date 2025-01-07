#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

class Mesh
{
public:
	glm::vec3 scale;
private:
	GLuint VAO, VBO, EBO;
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	int verticesCount = 0;
public:
	Mesh(glm::vec3 scale = { 10, 10, 0 })
	{
		this->scale = scale;
	}

	void CreatePlane();
	void CreateCube();

	void Draw(Shader& shader);
};