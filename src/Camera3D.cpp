#include "Camera3D.h"
#include "iostream"

glm::mat4 Camera3D::GetViewMatrix()
{
	return glm::lookAtRH(Position, Position + Front, Up);
}

void Camera3D::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}
