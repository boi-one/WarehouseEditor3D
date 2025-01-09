#include "Mouse.h"

void Mouse::Update(Camera2D& camera2d)
{
	position = { camera2d.ToWorldPosition(screenPosition), 0 };
}

void Mouse::SetScreenPosition(int x, int y)
{
	screenPosition = { x, y, 0 };
}

void Mouse::MouseMovement3D(SDL_Event& event, Camera3D& camera3d)
{
	if (CameraManager::orthoProjection) return;
	int offsetX = event.motion.xrel;
	int offsetY = event.motion.yrel;

	camera3d.yaw += sensitivity * offsetX;
	camera3d.pitch -= sensitivity * offsetY;

	if (camera3d.pitch > 89.0f) camera3d.pitch = 89.0f;
	if (camera3d.pitch < -89.0f) camera3d.pitch = -89.0f;

	glm::vec3 direction;
	direction.x = cos(glm::radians(camera3d.pitch)) * cos(glm::radians(camera3d.yaw));
	direction.z = sin(glm::radians(camera3d.pitch));
	direction.y = -cos(glm::radians(camera3d.pitch)) * sin(glm::radians(camera3d.yaw));
	camera3d.front = glm::normalize(direction);

	camera3d.right = glm::normalize(glm::cross(camera3d.front, camera3d.up));
}

void Mouse::Scroll(SDL_Event& event, Camera2D& camera2d)
{
	if (overUI) return;
	if (event.wheel.y < 0 && camera2d.zoom > 0.4f) camera2d.zoom -= 0.4f;
	if (event.wheel.y > 0 && camera2d.zoom < 8) camera2d.zoom += 0.4f;
}
