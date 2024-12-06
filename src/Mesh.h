#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Mesh
{
public:
	glm::vec3 position;

	Mesh(glm::vec3 position = glm::vec3(0, 0, 0))
	{
		this->position = position;
	}
};