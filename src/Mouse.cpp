#include "Mouse.h"

void Mouse::Update(Camera2D& camera2d)
{
	position = { camera2d.ToWorldPosition(screenPosition), 0 };
}

void Mouse::SetScreenPosition(int x, int y)
{
	screenPosition = { x, y, 0 };
}
