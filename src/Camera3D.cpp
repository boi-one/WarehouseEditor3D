#include "Camera3D.h"
#include "iostream"

glm::mat4 Camera3D::GetViewMatrix()
{
	return glm::lookAtRH(position, position + front, up);
}

void Camera3D::ProcessKeyboard(CameraMovement3D direction, float& deltaTime)
{
	float velocity = movementSpeed * deltaTime;
	if (direction == FORWARD)
		position += front * velocity;
	if (direction == BACKWARD)
		position -= front * velocity;
	if (direction == LEFT)
		position -= right * velocity;
	if (direction == RIGHT)
		position += right * velocity;
	if (direction == UP)
		position += up * velocity;
	if (direction == DOWN)
		position -= up * velocity;
}

