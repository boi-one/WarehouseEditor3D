#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Shader.h"

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
	int screenWidth = 1280;
	int screenHeight = 720;

	float cameraWidth;
	float cameraHeight;
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
	Viewport viewport;
	float zoom = 20;
	glm::mat4 transform;

	Camera2D(glm::vec3 position)
	{
		this->position = position;
		worldUp = up;
		UpdateCameraVectors();
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	glm::mat4 GetViewMatrix(); //voor 3d
	void Update();
	void SetTransform(Shader& shader);

	glm::vec3 ToWorldPosition(glm::vec3 position);

private:
	void UpdateCameraVectors();
};