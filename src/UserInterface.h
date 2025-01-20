#pragma once
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#include <vector>

#include "Settings.h"
#include "Mouse.h"
#include "Time.h"
#include "CameraManager.h"
#include "LayerManager.h"
#include "Tools.h"
using namespace Tools;
#include "JsonSerialization.h"

class UserInterface
{
public:
	Settings* settings = 0;
	Mouse* mouse = 0;
	CameraManager* cameraManager = 0;
	LayerManager* layerManager = 0;
	JsonSerialization* jsonSerialization = 0;
public:
	UserInterface(Settings* settings, Mouse* mouse, CameraManager* cameraManager, LayerManager* layerManager, JsonSerialization* jsonSerialization)
	{
		this->settings = settings;
		this->mouse = mouse;
		this->cameraManager = cameraManager;
		this->layerManager = layerManager;
		this->jsonSerialization = jsonSerialization;
	}
	void NewImGuiFrame();
	const char* SettingEnabled(bool b, const char* enabled = "enabled", const char* disabled = "disabled");
	void InterfaceInteraction(float deltaTime);

	void VisualSettings();
	void Camera3DSettings();

	void Layers(LayerManager& layerManager);
	void Conveyors(LayerManager& layerManager, Layer& currentLayer);
};