#pragma once
#include "Camera2D.h"

class Mouse
{
public:
	glm::vec3 position;
	void Update(Camera2D& camera2d);
	void SetScreenPosition(int x, int y);
private:
	glm::vec3 screenPosition;
};