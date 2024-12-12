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

	//to world space (je kan hier zelf bepalen wat de world coordinates zijn)
	//(nu is het half de scherm resolutie waar een zoom factor op is toegepast)
	//?? of niet want als ik de scherm resolutie aanpas hier klopt de muis positie niet meer vergeleken met de mesh scale
	mousePosition.x = mousePosition.x * viewport.cameraWidth / 2;
	mousePosition.y = mousePosition.y * viewport.cameraHeight/ 2;

	//offset voor als de camera beweegt
	mousePosition += glm::vec2(this->position.x, -this->position.y);

	mousePosition.y = -mousePosition.y;

	//std::cout << "screen l, r, t, d   : " << viewport.left << " " << viewport.right << " " << viewport.top << " " << viewport.bottom << " ";
	//std::cout << "mouse world position: " << mousePosition.x << " " << mousePosition.y << std::endl;
	
	//de uiteindelijke world position
	return mousePosition;

}

