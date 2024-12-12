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
	viewport.cameraWidth = viewport.screenWidth / zoom;
	viewport.cameraHeight = viewport.screenHeight / zoom;
	//
	//movementSpeed  zoom;
}

void Camera2D::SetTransform(Shader& shader)
{
	viewport.left = -viewport.cameraWidth / 2;
	viewport.right = viewport.cameraWidth / 2;
	viewport.bottom = -viewport.cameraHeight / 2;
	viewport.top = viewport.cameraHeight / 2;

	transform = glm::ortho(viewport.left + position.x, viewport.right + position.x, viewport.bottom + position.y, viewport.top + position.y, -100.0f, 100.0f);
	shader.setMat4("transform", transform);
}

glm::vec2 Camera2D::ToWorldPosition(glm::vec2 inputPosition)
{
	glm::vec2 mousePosition;

	//to clip space 0, 1
	mousePosition.x = inputPosition.x / viewport.screenWidth;
	mousePosition.y = inputPosition.y / viewport.screenHeight;

	//to cam space -1, 1
	mousePosition.x = (mousePosition.x - 0.5f) * 2;
	mousePosition.y = (mousePosition.y - 0.5f) * 2;

	//to world space (of wat je zelf wilt? idk vgm is het screen res???? of de cameraW/H)
	mousePosition.x = mousePosition.x * viewport.cameraWidth / 2;
	mousePosition.y = mousePosition.y * viewport.cameraHeight / 2;

	//offset voor als de camera beweegt
	mousePosition += glm::vec2(this->position.x, -this->position.y);

	mousePosition.y = -mousePosition.y;

	//std::cout << "screen l, r, t, d   : " << viewport.left << " " << viewport.right << " " << viewport.top << " " << viewport.bottom << " ";
	//std::cout << "mouse world position: " << mousePosition.x << " " << mousePosition.y << std::endl;
	
	//de uiteindelijke world position
	return mousePosition;

}

void Camera2D::UpdateCameraVectors()
{
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(this->front, worldUp));
	this->up = glm::normalize(glm::cross(this->right, this->front));
}
