#pragma once
#include <glm/glm.hpp>
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
	int windowWidth = 1280;
	int windowHeight = 720;

	float cameraWidth, cameraHeight;
	float left, right, top, bottom;

};

class Camera2D 
{
public:
	glm::vec3 position = glm::vec3(0, 0, 0);
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	glm::vec3 worldUp;
	float movementSpeed = 500.f;
	Viewport viewport;
	float zoom = 1;
	glm::mat4 projection;

	Camera2D(glm::vec3 position)
	{
		this->position = position;
	}

	void ProcessKeyboard(CameraMovement direction, float deltaTime);
	glm::mat4 GetViewMatrix(); //voor 3d
	void Update();
	void SetTransform(Shader& shader);

	glm::vec2 ToWorldPosition(glm::vec2 position);
};