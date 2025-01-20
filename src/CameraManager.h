#pragma once
#include "Camera2D.h"
#include "Camera3D.h"

/// <summary>
/// Manages the multiple cameras and keeps track of the projection
/// </summary>
class CameraManager
{
public:
	bool orthoProjection = true;
	Camera2D camera2d;
	Camera3D camera3d;

public:
	/// <summary>
	/// sets the mouse visible when needed and updates the camera transforms
	/// </summary>
	/// <param name="shader"> needed for the projection matrix </param>
	/// <param name="openSettings"> decides when the mouse should be visible or not </param>
	void UpdateProjection(Shader& shader, bool& openSettings);
};