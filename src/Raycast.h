#pragma once
#include "glm\glm.hpp"
#include "CameraManager.h"
#include "Mouse.h"

class Raycast
{
public:
	glm::vec3 origin;
	glm::vec3 direction;
	glm::vec3 intersection;
	Mouse* mouse = 0;
	CameraManager* cameraManager = 0;
	Viewport* viewport = 0;

	Raycast(glm::vec3 direction, Mouse* mouse, CameraManager* cameraManager, glm::vec3 origin = { 0,0,0 })
	{
		this->origin = origin;
		this->direction = direction;
		this->mouse = mouse;
		this->cameraManager = cameraManager;
		this->viewport = &cameraManager->camera2d.viewport;
	}

public:
	void Ray();
	bool Intersection(float planeHeight);
};