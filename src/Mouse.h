#pragma once
#include "Camera2D.h"
#include <SDL.h>

class Mouse
{
public:
	glm::vec3 position;
	float horizontalAngle = 3.14f;
	float verticalAngle = 0.0f;

public:
	void Update(Camera2D& camera2d);
	void SetScreenPosition(int x, int y);
	void Rotate(SDL_Window* window, Camera2D& camera2d);
private:
	/// <summary>
	/// the mouse position in pixel coordinates with top left being 0, 0 and bottom right max screen width and screen height
	/// </summary>
	glm::vec3 screenPosition;
};