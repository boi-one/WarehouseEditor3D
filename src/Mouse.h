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
	/// be careful because this is not set automatically unlike the rest of the places where the cameraManager is passed as a pointer
	/// (in this case it is set in the Input constructor)
	/// </summary>
	CameraManager* cameraManager = nullptr;
public:
	/// <summary>
	/// updates the position
	/// </summary>
	void Update(Camera2D& camera2d, int cellSize);
	/// <summary>
	/// moves the mouse to x y
	/// </summary>
	void SetScreenPosition(int x, int y);
	/// <summary>
	/// used to look around in 3D
	/// </summary>
	/// <param name="event">used to check if the mouse is moving and which direction</param>
	void MouseMovement3D(SDL_Event& event, Camera3D& camera3d);
	/// <summary>
	/// zooms in and out in 2D
	/// </summary>
	/// <param name="event"> used for the scrollwheel </param>
	void Scroll(SDL_Event& event, Camera2D& camera2d);
	/// <summary>
	/// snaps the mouse to the grid
	/// </summary>
	/// <param name="cellSize">how big a cell in the grid is</param>
	void UpdateMouseOnGrid(int cellSize);

	/// <summary>
	/// the mouse position in pixel coordinates with top left being 0, 0 and bottom right max screen width and screen height
	/// </summary>
	glm::vec3 screenPosition;
};
