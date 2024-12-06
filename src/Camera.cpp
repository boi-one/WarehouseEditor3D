#include "Camera.h"
#include <iostream>

void Camera2D::ProcessKeyboard(CameraMovement direction, float deltaTime)
{
	float velocity = movementSpeed * deltaTime;

	switch (direction)
	{
	case CameraMovement::Forward:
	{
		position += front * velocity;
	}break;
	case CameraMovement::Backward:
	{
		position -= front * velocity;
	}break;
	case CameraMovement::Left:
	{
		position -= right * velocity;
	}break;
	case CameraMovement::Right:
	{
		position += right * velocity;
	}break;
	case CameraMovement::Up:
	{
		position += up * velocity;
	}break;
	case CameraMovement::Down:
	{
		position -= up * velocity;
	}break;
	}
}

glm::mat4 Camera2D::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera2D::UpdateCameraVectors()
{
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
