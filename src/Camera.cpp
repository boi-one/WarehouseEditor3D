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

void Camera2D::Update()
{
	std::cout << zoom << std::endl;
	viewport.cameraWidth = viewport.screenWidth / zoom;
	viewport.cameraHeight = viewport.screenHeight / zoom;

	//movementSpeed /= zoom;
}

void Camera2D::SetTransform(Shader& shader)
{
	float width = (float)viewport.screenWidth / zoom;
	float height = (float)viewport.screenHeight / zoom;

	transform = glm::ortho(-width / 2 + position.x, width / 2 + position.x, -height / 2 + position.y, height / 2 + position.y, -100.0f, 100.0f);
	shader.setMat4("transform", transform);
}

glm::vec3 Camera2D::ToWorldPosition(glm::vec3 position)
{
	glm::vec3 mousePosition;

	//to clip space 0, 1
	mousePosition.x = position.x / viewport.screenWidth;
	mousePosition.y = position.y / viewport.screenHeight;

	//to cam space -1, 1
	mousePosition.x = (mousePosition.x - 0.5f) * 2;
	mousePosition.y = (mousePosition.y - 0.5f) * 2;

	//to world space -500 ,500 (of wat je zelf wilt? idk vgm is het screen res???? of de cameraW/H)
	mousePosition.x = mousePosition.x * viewport.cameraWidth / 2;
	mousePosition.y = mousePosition.y * viewport.cameraHeight / 2;

	//offset voor als de camera beweegt
	//mousePosition * glm::vec3(position.x, position.y, 0);

	//de uiteindelijke world position
	return mousePosition;

}

void Camera2D::UpdateCameraVectors()
{
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
