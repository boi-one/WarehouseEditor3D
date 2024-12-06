#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement
{
	Forward,
	Backward,
	Left,
	Right,
	Up,
	Down
};

struct Viewport
{
	unsigned int screenWidth = 1280;
	unsigned int screenHeight = 720;

	float top = 0;
	float bottom = 0;
	float left = screenWidth;
	float right = screenHeight;
};

class Camera2D 
{
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	glm::vec3 worldUp;
	float movementSpeed = 1500.f;
	float fov = 45.0f;
	Viewport viewport;

	Camera2D(glm::vec3 position)
	{
		this->position = position;
		worldUp = up;
		UpdateCameraVectors();
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	glm::mat4 GetViewMatrix();

private:
	void UpdateCameraVectors();
};