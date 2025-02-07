#include "Camera2D.h"
#include <iostream>

/// <summary>
/// moves the camera
/// </summary>
/// <param name="direction">movement direction</param>
/// <param name="deltaTime">used for smooth movement on different computers</param>
void Camera2D::ProcessKeyboard(CameraMovement direction, float& deltaTime)
{
	float velocity = movementSpeed / zoom * 2 * deltaTime;

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
/// <summary>
/// updates the camera
/// </summary>
void Camera2D::Update()
{
	viewport.cameraWidth = viewport.windowWidth / zoom;
	viewport.cameraHeight = viewport.windowHeight / zoom;
	pixelSize = 1 / zoom;
}
/// <summary>
/// sets the projection and view matrices
/// </summary>
/// <param name="shader">the shader the matrices are send to</param>
void Camera2D::SetTransform(Shader& shader)
{
	viewport.left = -viewport.cameraWidth / 2;
	viewport.right = viewport.cameraWidth / 2;
	viewport.bottom = -viewport.cameraHeight / 2;
	viewport.top = viewport.cameraHeight / 2;

	projection = glm::ortho(viewport.left + position.x, viewport.right + position.x, viewport.bottom + position.y, viewport.top + position.y, -1000.0f, 10.0f);	
	shader.setMat4("projection", projection);

	glm::vec3 worldPosition;
	glm::mat4 view = glm::mat4(1.f);
	shader.setMat4("view", view);
}
/// <summary>
/// converts a point to world position
/// </summary>
/// <param name="inputPosition">the pixel coordinates</param>
/// <returns>the world position</returns>
glm::vec2 Camera2D::ToWorldPosition(glm::vec2 inputPosition)
{
	glm::vec2 screenPosition;

	//to clip space 0, 1
	screenPosition.x = inputPosition.x / viewport.windowWidth;
	screenPosition.y = inputPosition.y / viewport.windowHeight;

	//to cam space -1, 1
	screenPosition.x = (screenPosition.x - 0.5f) * 2;
	screenPosition.y = (screenPosition.y - 0.5f) * 2;

	//to world space (you can determine the world coordinates yourself here, but keep /2 because the 0 point is in the middle, otherwise you don't have to do /2 if the 0 point is in the corner)

	float width = viewport.cameraWidth;
	float height = viewport.cameraHeight;
	screenPosition.x = screenPosition.x * width / 2;
	screenPosition.y = screenPosition.y * height / 2;

	//offset if the camera moves
	screenPosition += glm::vec2(this->position.x, -this->position.y);

	screenPosition.y = -screenPosition.y;

	//the eventual world position
	return screenPosition;

}

