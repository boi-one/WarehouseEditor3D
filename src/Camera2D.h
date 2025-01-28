#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

enum CameraMovement
{
	Left,
	Right,
	Up,
	Down
};

struct Viewport
{
	/// <summary>
	/// size of the window (amount of pixels width and height)
	/// </summary>
	int windowWidth = 1280;
	/// /// <summary>
	/// size of the window (amount of pixels width and height)
	/// </summary>
	int windowHeight = 720;

	/// <summary>
	/// size of the virtual camera, not the actual pixel size
	/// </summary>
	float cameraWidth, cameraHeight;
	float left{0}, right{0}, top{0}, bottom{0};

};

/// <summary>
/// Z is up and Y is depth in the current coordinate system
/// </summary>
class Camera2D 
{
public:
	glm::vec3 position = glm::vec3(0, 0, 3);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right = glm::vec3();
	glm::vec3 worldUp = glm::vec3();
	float baseMovementSpeed = 0;
	float movementSpeed = 500.f;
	Viewport viewport;
	float zoomMin = 0.25f;
	float zoomMax = 2.0f;
	float zoom = 1;
	glm::mat4 projection = glm::mat4();
	float pixelSize = 0;

	Camera2D(glm::vec3 position = { 0.0f, 0.0f, 1.0f })
	{
		this->position = position;
		baseMovementSpeed = movementSpeed;
	}
	/// <summary>
	/// makes the camera move in 2D
	/// </summary>
	/// <param name="direction">the direction the camera should move in</param>
	/// <param name="deltaTime">used for smooth movement across different computers</param>
	void ProcessKeyboard(CameraMovement direction, float& deltaTime);
	/// <summary>
	/// Update viewport width and height for when you resize the window
	/// </summary>
	void Update();
	/// <summary>
	/// updates the projection based on the viewport
	/// </summary>
	/// <param name="shader">used for transforms</param>
	void SetTransform(Shader& shader);
	/// <summary>
	/// converts a point from screen position to world position
	/// </summary>
	/// <param name="position">screen position</param>
	/// <returns>world position</returns>
	glm::vec2 ToWorldPosition(glm::vec2 position);
};