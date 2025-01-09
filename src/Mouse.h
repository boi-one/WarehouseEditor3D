#pragma once
#include "CameraManager.h"
#include <SDL.h>

class Mouse
{
public:
	glm::vec3 position;
	float sensitivity = 0.4f;
	bool overUI = false;
public:
	void Update(Camera2D& camera2d);
	void SetScreenPosition(int x, int y);
	void MouseMovement3D(SDL_Event& event, Camera3D& camera3d);
	void Scroll(SDL_Event& event, Camera2D& camera2d);
private:
	/// <summary>
	/// the mouse position in pixel coordinates with top left being 0, 0 and bottom right max screen width and screen height
	/// </summary>
	glm::vec3 screenPosition;
};