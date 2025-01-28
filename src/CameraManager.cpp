#include "CameraManager.h"

void CameraManager::UpdateProjection(Shader& shader, bool& openSettings)
{
	if (orthoProjection)
	{
		camera2d.SetTransform(shader);	
	}
	else
	{
		camera3d.SetTransform(shader, (float)camera2d.viewport.cameraWidth, (float)camera2d.viewport.cameraHeight);
	}
}
