#include "Mouse.h"

void Mouse::Update(Camera2D& camera2d)
{
	position = { camera2d.ToWorldPosition(screenPosition), 0 };
}

void Mouse::SetScreenPosition(int x, int y)
{
	screenPosition = { x, y, 0 };
}

void Mouse::Rotate(SDL_Window* window, Camera2D& camera2d)
{
	SDL_WarpMouseInWindow(window, camera2d.viewport.cameraWidth / 2, camera2d.viewport.cameraHeight / 2);
	//TODO: maak dit af
}
