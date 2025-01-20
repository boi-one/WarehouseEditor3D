#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SDL.h>
#include <math.h>
#include "Camera2D.h"


enum CameraMovement3D {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};
/// <summary>
/// Z is up and Y is depth in the current coordinate system
/// </summary>
class Camera3D
{
public:
	glm::vec3 position;
	glm::vec3 front = glm::vec3();
	glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 up = glm::vec3(0.0f, 0.0f, 1.0f);
	float movementSpeed = 150.f;
	float fov = 45.f;
	float pitch = 0.f;
	float yaw = 0.f;
public:
	Camera3D(glm::vec3 position = glm::vec3(-100.0f, 0.0f, 100.0f), glm::vec3 front = glm::vec3(0.0f, 1.0f, 0.0f))
	{
		this->front = front;
		this->position = position;
	}
	/// <summary>
	/// used for looking around
	/// </summary>
	/// <returns>the view matrix</returns>
	glm::mat4 GetViewMatrix();
	/// <summary>
	/// makes the camera move in 2D
	/// </summary>
	/// <param name="direction">the direction the camera should move in</param>
	/// <param name="deltaTime">used for smooth movement across different computers</param>
	void ProcessKeyboard(CameraMovement3D direction, float& deltaTime);
	/// <summary>
	/// updates the projection and view matrices
	/// </summary>
	/// <param name="shader">used for transforms</param>
	/// <param name="cameraWidth">the width of the camera</param>
	/// <param name="cameraHeight">the height of the camera</param>
	void SetTransform(Shader& shader, float cameraWidth, float cameraHeight);

};