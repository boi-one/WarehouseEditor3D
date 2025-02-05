#pragma once
#include "CameraManager.h"
#include "math.h"
#include "Tools.h"
#include <SDL.h>

class Mouse
{
public:
	glm::vec3 position = glm::vec3(0);
	glm::vec3 gridPosition = glm::vec3(0);
	glm::vec3 newLineEnd;
	float sensitivity = 0.4f;
	bool overUI = false;
	glm::vec3 dragOffset = glm::vec3(0, 0, 0);
	bool middleMouseFirstPress = false;
	bool middleMousePressed = false;
	/// <summary>
	/// be carefull because this is not set automaticly unlike the rest of the places where the cameraManager is passed as a pointer
	/// (in this case it is set in the Input constructor)
	/// </summary>
	CameraManager* cameraManager = nullptr;
public:
	void Update(Camera2D& camera2d, int cellSize);
	void SetScreenPosition(int x, int y);
	void MouseMovement3D(SDL_Event& event, Camera3D& camera3d);
	void Scroll(SDL_Event& event, Camera2D& camera2d);
	void UpdateMouseOnGrid(int cellSize);

	/// <summary>
	/// the mouse position in pixel coordinates with top left being 0, 0 and bottom right max screen width and screen height
	/// </summary>
	glm::vec3 screenPosition;
};