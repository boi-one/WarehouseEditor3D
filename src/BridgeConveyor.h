#pragma once
#include "Mesh.h"

class BridgeConveyor 
{
public:
	glm::vec3 start = glm::vec3();
	glm::vec3 end = glm::vec3();
	float width = 0;
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;

	BridgeConveyor(glm::vec3 start, glm::vec3 end, float width = 10, Shader* shader = nullptr)
	{
		this->start = start;
		this->end = end;
		this->shader = shader;
		this->width = width;
	}

	void Draw();
};