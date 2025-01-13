#pragma once
#include "Camera2D.h"
#include "Camera3D.h"

class CameraManager
{
public:
	bool orthoProjection = true;
	Camera2D camera2d;
	Camera3D camera3d;

public:
	void UpdateProjection(Shader& shader, bool& openSettings);
};