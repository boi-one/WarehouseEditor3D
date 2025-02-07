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
	Mouse* mouse = nullptr;
	CameraManager* cameraManager = nullptr;
	Viewport* viewport = nullptr;

	Raycast(glm::vec3 direction, Mouse* mouse, CameraManager* cameraManager, glm::vec3 origin = { 0,0,0 })
	{
		this->origin = origin;
		this->direction = direction;
		this->mouse = mouse;
		this->cameraManager = cameraManager;
		this->viewport = &cameraManager->camera2d.viewport;
	}

public:
	/// <summary>
	/// "shoots" the ray
	/// </summary>
	void Ray();
	/// <summary>
	/// checks if it get hit on the specific height
	/// </summary>
	/// <param name="planeHeight"> the height</param>
	/// <returns>true or false depending on if it hit</returns>
	bool Intersection(float planeHeight);
};