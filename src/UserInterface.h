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

/// <summary>
/// handles the interface using ImGui
/// </summary>
class UserInterface
{
public:
	Settings* settings = 0;
	Mouse* mouse = 0;
	CameraManager* cameraManager = 0;
	LayerManager* layerManager = 0;
	JsonSerialization* jsonSerialization = 0;
	bool saved = false;
	int popupTime;
public:
	UserInterface(Settings* settings, Mouse* mouse, CameraManager* cameraManager, LayerManager* layerManager, JsonSerialization* jsonSerialization)
	{
		this->settings = settings;
		this->mouse = mouse;
		this->cameraManager = cameraManager;
		this->layerManager = layerManager;
		this->jsonSerialization = jsonSerialization;
	}
	/// <summary>
	/// handles some imgui functions
	/// </summary>
	void NewImGuiFrame();
	/// <summary>
	/// displays if the specific setting is enabled or disabled
	/// </summary>
	/// <param name="b">the bool to check if the setting is enabled or disabled</param>
	/// <param name="enabled">text you want to display when setting is enabled</param>
	/// <param name="disabled">text you want to display when setting is disabled</param>
	/// <returns>the 'string' you want to display</returns>
	const char* SettingEnabled(bool b, const char* enabled = "enabled", const char* disabled = "disabled");
	/// <summary>
	/// displays the imgui windows
	/// </summary>
	/// <param name="deltaTime">used for the FPS counter</param>
	void InterfaceInteraction(float deltaTime);

	/// <summary>
	/// renders the elements for the visual settings
	/// </summary>
	void VisualSettings();
	/// <summary>
	/// renders the elements for the camera settings
	/// </summary>
	void Camera3DSettings();
	/// <summary>
	/// renders the layer panel
	/// </summary>
	/// <param name="layerManager">used to pass the layers vector</param>
	void Layers(LayerManager& layerManager);
	/// <summary>
	/// used to render the list of conveyors pers layer
	/// </summary>
	/// <param name="layerManager">used to pass the layers vector</param>
	/// <param name="currentLayer">the layer currently being looped over</param>
	void Conveyors(LayerManager& layerManager, Layer& currentLayer);
};