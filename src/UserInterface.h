#pragma once
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

#include "Settings.h"
#include "Mouse.h"
#include "Time.h"
#include "CameraManager.h"

class UserInterface
{
public:
	Settings* settings = 0;
	Mouse* mouse = 0;
	CameraManager* cameraManager = 0;
public:
	UserInterface(Settings* settings, Mouse* mouse, CameraManager* cameraManager)
	{
		this->settings = settings;
		this->mouse = mouse;
		this->cameraManager = cameraManager;
	}
	void NewImGuiFrame();
	const char* SettingEnabled(bool b, const char* enabled = "enabled", const char* disabled = "disabled");
	void InterfaceInteraction(float deltaTime);

	void VisualSettings();
	void Camera3DSettings();
};