#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

class Mesh
{
public:
	glm::vec3 position;
	glm::vec3 scale;
private:
	GLuint VAO;
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	int verticesCount = 0;
public:
	Mesh(glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 10, 10, 0 })
	{
		this->position = position;
		this->scale = scale;
		this->VAO = VAO;
	}

	void CreatePlane();
	void CreateCube();

	void Draw(Shader& shader);
};