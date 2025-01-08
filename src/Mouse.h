#pragma once
#include "Camera2D.h"

class Mouse
{
public:
	glm::vec3 position;
	float sensitivity = 0.4f;
public:
	void Update(Camera2D& camera2d);
	void SetScreenPosition(int x, int y);
private:
	/// <summary>
	/// the mouse position in pixel coordinates with top left being 0, 0 and bottom right max screen width and screen height
	/// </summary>
	glm::vec3 screenPosition;
};