#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>

class Mesh
{
public:
	glm::vec3 position;
	glm::vec3 scale;

	GLuint VBO;

	Mesh(GLuint VBO, glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 20, 20, 0 })
	{
		this->position = position;
		this->scale = scale;
		this->VBO = VBO;
	}

	void Draw();
};