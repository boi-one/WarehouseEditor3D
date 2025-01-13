#include "CameraManager.h"

void CameraManager::UpdateProjection(Shader& shader, bool& openSettings)
{
	if (orthoProjection)
	{
		camera2d.SetTransform(shader);
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
	else
	{
		if (openSettings) SDL_SetRelativeMouseMode(SDL_TRUE);

		camera3d.SetTransform(shader, (float)camera2d.viewport.cameraWidth, (float)camera2d.viewport.cameraHeight);
	}
}
