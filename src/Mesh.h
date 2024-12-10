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
	GLuint VBO, VAO;

	Mesh(GLuint VBO, GLuint VAO, glm::vec3 position = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 0 })
	{
		this->position = position;
		this->scale = scale;
		this->VBO = VBO;
		this->VAO = VAO;
	}


	void Draw(std::vector<GLfloat> vertices, GLuint& EBO, GLuint indices[6] = 0);
};