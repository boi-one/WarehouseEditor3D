#include "Camera2D.h"
#include <iostream>

void Camera2D::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;

	switch (direction)
	{
	case CameraMovement::Left:
	{
		position -= glm::vec3(1, 0, 0) * velocity;
	}break;
	case CameraMovement::Right:
	{
		position += glm::vec3(1, 0, 0) * velocity;
	}break;
	case CameraMovement::Up:
	{
		position += glm::vec3(0, 1, 0) * velocity;
	}break;
	case CameraMovement::Down:
	{
		position -= glm::vec3(0, 1, 0) * velocity;
	}break;
	}
}

glm::mat4 Camera2D::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera2D::Update()
{
	viewport.cameraWidth = viewport.windowWidth / zoom;
	viewport.cameraHeight = viewport.windowHeight / zoom;
	//
	//movementSpeed  zoom;
}

void Camera2D::SetTransform(Shader& shader)
{
	viewport.left = -viewport.cameraWidth / 2;
	viewport.right = viewport.cameraWidth / 2;
	viewport.bottom = -viewport.cameraHeight / 2;
	viewport.top = viewport.cameraHeight / 2;

	projection = glm::ortho(viewport.left + position.x, viewport.right + position.x, viewport.bottom + position.y, viewport.top + position.y, -100.0f, 100.0f);
	shader.setMat4("projection", projection);

	glm::vec3 worldPosition;
	glm::mat4 view = glm::mat4(1.f);
	shader.setMat4("view", view);
}

glm::vec2 Camera2D::ToWorldPosition(glm::vec2 inputPosition)
{
	glm::vec2 screenPosition;

	//to clip space 0, 1
	screenPosition.x = inputPosition.x / viewport.windowWidth;
	screenPosition.y = inputPosition.y / viewport.windowHeight;

	//to cam space -1, 1
	screenPosition.x = (screenPosition.x - 0.5f) * 2;
	screenPosition.y = (screenPosition.y - 0.5f) * 2;

	//to world space (je kan hier zelf bepalen wat de world coordinates zijn maar wel /2 houden omdat het 0 punt in het midden is, anders hoef je niet /2 te doen als het 0 punt in de hoek is)

	float width = viewport.cameraWidth;
	float height = viewport.cameraHeight;
	screenPosition.x = screenPosition.x * width / 2;
	screenPosition.y = screenPosition.y * height / 2;

	//offset voor als de camera beweegt
	screenPosition += glm::vec2(this->position.x, -this->position.y);

	screenPosition.y = -screenPosition.y;

	//de uiteindelijke world position
	return screenPosition;

}

