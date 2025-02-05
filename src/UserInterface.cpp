#include "UserInterface.h"
#include "imgui_stdlib.h"

void UserInterface::NewImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

const char* UserInterface::SettingEnabled(bool b, const char* enabled, const char* disabled)
{
	if (b) return enabled;
	return disabled;
}

void UserInterface::InterfaceInteraction(float deltaTime)
{
	mouse->overUI = false;
	if (settings->showInfo)
	{
		ImGui::Begin("Info", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
		mouse->overUI |= ImGui::IsWindowHovered();
		ImGui::SetWindowPos({ 10, 10 });
		ImGui::SetWindowSize({ 310, 150 });
		ImGui::Text("- FPS: %.0f", 1 / deltaTime);
		ImGui::Text("- Mouse position: X %.0f, Y %.0f", mouse->position.x, mouse->position.y);
		ImGui::Text("- 2D Camera position: X %.0f, Y %.0f", cameraManager->camera2d.position.x, cameraManager->camera2d.position.y);
		ImGui::Text("- 3D Camera position: X %.0f, Y %.0f, Z %.0f", cameraManager->camera3d.position.x, cameraManager->camera3d.position.y, cameraManager->camera3d.position.z);
		ImGui::Text("- Screen resolution: X %d, Y %d", cameraManager->camera2d.viewport.windowWidth, cameraManager->camera2d.viewport.windowHeight);
		ImGui::End();
	}

	if (settings->openSettings)
	{
		ImGui::Begin("Settings & Help", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		mouse->overUI |= ImGui::IsWindowHovered();
		ImGui::SetWindowSize({ 600, 300 });
		glm::vec2 centerScreen((float)cameraManager->camera2d.viewport.windowWidth / 2, (float)cameraManager->camera2d.viewport.windowHeight / 2);
		ImGui::SetWindowPos({ centerScreen.x - ImGui::GetWindowSize().x / 2, centerScreen.y - ImGui::GetWindowSize().y / 2 });

		if (SDL_GetTicks() < popupTime && saved)
		{
			ImGui::Text("Successfully saved!");
		}
		else saved = false;
		if (ImGui::Button("Save"))
		{
			jsonSerialization->Serialize(layerManager->allLayers, layerManager->allBridgeConveyors);
			saved = true;
			popupTime = SDL_GetTicks() + 1000;
		}
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			std::string path = "save.json";
			jsonSerialization->Deserialize(path, *layerManager);
			settings->openSettings = false;
		}
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { .75f, 0, 0, 1 });
		if (ImGui::Button("Quit")) settings->appRunning = false;
		if (ImGui::IsItemHovered()) ImGui::SetTooltip("Quit the application, progress won't be saved");
		ImGui::PopStyleColor();
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SeparatorText("Visuals");

			ImGui::Spacing();
			VisualSettings();
			ImGui::Spacing();

			CameraSettings();
			ImGui::Spacing();

			ImGui::SeparatorText("Grid size");
			ImGui::Spacing();
			ImGui::PushID("grid");
			if (ImGui::Button("Default")) grid->gridSize = 45;
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::SetNextItemWidth(220);
			ImGui::SliderInt("grid size", &grid->gridSize, 4, 1000);
			if (ImGui::IsItemHovered()) ImGui::SetTooltip("use this when the current grid is too small for your project");
			ImGui::Spacing();
		}

		if (ImGui::CollapsingHeader("Keybindings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::SeparatorText("General Keybindings");
			ImGui::Spacing();
			ImGui::Text("Esc         : Open settings menu");
			ImGui::Text("Tab         : Switch projection");
			ImGui::Text("I           : Show info");
			ImGui::Text("L           : Show layers");
			ImGui::Text("R           : Resets the active camera to the default position");
			ImGui::Text("Left Ctrl   : Increase the movement speed of the active camera");
			ImGui::Text("Z           : Unselect Conveyor");
			ImGui::Text("Left Shift  : Merge conveyors together");
			ImGui::Text("X           : Unselect point");
			ImGui::Text("G           : Enable grid snapping");
			ImGui::Text("Alt + Delete: Deletes the selected conveyor");
			ImGui::Text("Space       : Connect conveyors on different layers");
			ImGui::Text("Delete      : Deletes the selected point in a conveyor");
			ImGui::Text("Left Arrow  : Rotates the conveyor counter clockwise");
			ImGui::Text("Right Arrow : Rotates the conveyor clockwise");

			ImGui::Spacing();
			ImGui::SeparatorText("2D Keybindings");
			ImGui::Spacing();
			ImGui::Text("W           : Move up");
			ImGui::Text("A           : Move left");
			ImGui::Text("S           : Move down");
			ImGui::Text("D           : Move right");

			ImGui::Spacing();
			ImGui::SeparatorText("3D Keybindings");
			ImGui::Spacing();
			ImGui::Text("W           : Move forward");
			ImGui::Text("A           : Strafe left");
			ImGui::Text("S           : Move backwards");
			ImGui::Text("D           : Strafe right");
			ImGui::Text("Left Shift  : Move down");
			ImGui::Text("Space       : Move up");
		}
		ImGui::End();
	}

	Layers(*layerManager);
}

void UserInterface::VisualSettings()
{
	if (ImGui::Button("Show Axes")) settings->showAxes = !settings->showAxes;
	ImGui::SameLine();
	ImGui::Text(SettingEnabled(settings->showAxes));
	if (ImGui::Button("Show Grid")) settings->showGrid = !settings->showGrid;
	ImGui::SameLine();
	ImGui::Text(SettingEnabled(settings->showGrid));
	if (ImGui::Button("Snap to Grid")) settings->gridSnap = !settings->gridSnap;
	ImGui::SameLine();
	ImGui::Text(SettingEnabled(settings->gridSnap));

	if (ImGui::Button("Wireframe"))
	{
		settings->wireframe = !settings->wireframe;
		if (settings->wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	ImGui::SameLine();
	ImGui::Text(SettingEnabled(settings->wireframe));

	if (ImGui::Button("Camera Projection")) cameraManager->orthoProjection = !cameraManager->orthoProjection;
	ImGui::SameLine();
	ImGui::Text(SettingEnabled(cameraManager->orthoProjection, "Orthographic", "Perspective"));
}

void UserInterface::CameraSettings()
{
	ImGui::SeparatorText("3D Camera");
	ImGui::Spacing();

	float fovMin = 10.f;
	float fovMax = 130.f;
	float sliderWidth = 220;

	ImGui::PushID("fov");
	if (ImGui::Button("Default")) cameraManager->camera3d.fov = 45.f;
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat("FOV        ", &cameraManager->camera3d.fov, fovMin, fovMax);
	float sensMin = 0.01f;
	float sensMax = 1.f;
	ImGui::PushID("sens");
	if (ImGui::Button("Default")) mouse->sensitivity = 0.4f;
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat("Sensitivity", &mouse->sensitivity, sensMin, sensMax);
	ImGui::SeparatorText("2D Camera");
	ImGui::Spacing();

	ImGui::PushID("zoom");
	if (ImGui::Button("Default")) cameraManager->camera2d.zoom = 1;
	ImGui::PopID();
	ImGui::SameLine();
	ImGui::SetNextItemWidth(sliderWidth);
	ImGui::SliderFloat("Zoom       ", &cameraManager->camera2d.zoom, cameraManager->camera2d.zoomMin, cameraManager->camera2d.zoomMax);

	ImGui::Spacing();
}

void UserInterface::Layers(LayerManager& layerManager)
{
	if (!settings->showLayers) return;
	ImGui::Begin("Layers", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNav);
	mouse->overUI |= ImGui::IsWindowHovered();
	ImGui::SetWindowPos({ 10, (float)cameraManager->camera2d.viewport.windowHeight - ImGui::GetWindowSize().y - 10 });
	ImGui::SetWindowSize({ 310, (float)cameraManager->camera2d.viewport.windowHeight / 2 });
	std::vector<int> deletions;

	if (ImGui::Button("add layer"))
	{
		layerManager.AddLayer();
	}

	for (int i = 0; i < layerManager.allLayers.size(); i++)
	{
		std::vector<Layer>& allLayers = layerManager.allLayers;
		allLayers[i].SetDepth(i);

		ImGui::PushID(allLayers[i].ID());
		char layerLabel[128] = {};
		snprintf(layerLabel, sizeof(layerLabel), "%d. Layer: %d\nconveyor amount: %d", i, (int)allLayers[i].ID(), (int)allLayers[i].allConveyors.size());
		if (allLayers[i].selected) ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, .75f, 0, 1));
		if (ImGui::CollapsingHeader(layerLabel))
		{
			Conveyors(layerManager, allLayers[i]);
		}
		if (allLayers[i].selected) ImGui::PopStyleColor();
		ImGui::PushID(i);
		if (ImGui::CollapsingHeader("edit conveyor width") && allLayers[i].allConveyors.size() > 0)
		{
			ImGui::SliderFloat("width", &allLayers[i].width, allLayers[i].allConveyors[0].widthMin, allLayers[i].allConveyors[0].widthMax);
			if (ImGui::Button("set width")) for (Conveyor& c : allLayers[i].allConveyors) c.width = allLayers[i].width;
		}
		if (ImGui::Button("move up") && i > 0)
		{
			if (&allLayers[i] == layerManager.selectedLayer)
				allLayers[i].UnselectConveyors();
			Layer tempLayer = allLayers[i];
			Layer tempLayerPrev = allLayers[i - 1];
			allLayers[i - 1] = tempLayer;
			allLayers[i] = tempLayerPrev;
			for (Layer& l : allLayers) if (l.selected) layerManager.selectedLayer = &l;
			for (BridgeConveyor& bg : layerManager.allBridgeConveyors) layerManager.UpdateBridgeConveyors(bg);
		}
		ImGui::SameLine();
		if (ImGui::Button("move down") && i < allLayers.size() - 1)
		{
			if (&allLayers[i] == layerManager.selectedLayer)
				allLayers[i].UnselectConveyors();
			Layer tempLayer = allLayers[i];
			Layer tempLayerPrev = allLayers[i + 1];
			allLayers[i + 1] = tempLayer;
			allLayers[i] = tempLayerPrev;
			for (Layer& l : allLayers) if (l.selected) layerManager.selectedLayer = &l;
			for (BridgeConveyor& bg : layerManager.allBridgeConveyors) layerManager.UpdateBridgeConveyors(bg);
		}
		ImGui::SameLine();
		if (ImGui::Button("select"))
		{
			layerManager.UnselectEverything();
			allLayers[i].selected = true;
			layerManager.selectedLayer = &allLayers[i];
		}
		ImGui::SameLine();
		if (ImGui::Button("delete"))
		{
			if (!allLayers[i].selected)
				deletions.push_back(i);
		}
		ImGui::PopID();
		if (ImGui::IsItemHovered() && allLayers[i].selected)
		{
			ImGui::SetTooltip("cannot be deleted because this is currently the selected layer");
		}
		ImGui::SameLine();
		ImGui::Checkbox("", &allLayers[i].hidden);
		if (ImGui::IsItemHovered() && allLayers[i].selected)
		{
			ImGui::SetTooltip("hide layer");
		}
		ImGui::Spacing();
		ImGui::PopID();
	}
	for (int i : deletions)
		DeleteFromList(layerManager.allLayers, layerManager.allLayers[i]);
	for (Layer& l : layerManager.allLayers) if (l.selected) layerManager.selectedLayer = &l;

	ImGui::End();
}

void UserInterface::Conveyors(LayerManager& layerManager, Layer& currentLayer)
{
	std::vector<int> deletions;
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0, 0, 0));
	std::vector<Conveyor>& allConveyors = currentLayer.allConveyors;
	for (int j = 0; j < allConveyors.size(); j++)
	{
		char layerLabel[128] = {};
		char conveyorLabel[128] = {};
		snprintf(conveyorLabel, sizeof(conveyorLabel), "%d. Conveyor: %d", j, (int)allConveyors[j].ID());
		if (allConveyors[j].selected) ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, 0.4f, 0, 1));
		ImGui::PushID(&allConveyors[j]);
		if (ImGui::CollapsingHeader(conveyorLabel))
		{
			for (Point& point : allConveyors[j].path)
			{
				char pointLabel[128] = {};
				snprintf(pointLabel, sizeof(layerLabel), "point: %d X: %0.f Y: %0.f", point.ID(), point.position.x, point.position.y);
				if (ImGui::Button(pointLabel))
				{
					cameraManager->camera2d.position = point.position;
				}
			}
		}
		if (allConveyors[j].selected) ImGui::PopStyleColor();
		if (ImGui::Button("select") && &currentLayer == layerManager.selectedLayer)
		{
			currentLayer.UnselectConveyors();
			allConveyors[j].selected = true;
			layerManager.selectedLayer->selectedConveyor = &allConveyors[j];
		}
		ImGui::SameLine();
		if (ImGui::Button("delete"))
		{
			if (!allConveyors[j].selected)
				deletions.push_back(j);
		}
		if (ImGui::IsItemHovered() && allConveyors[j].selected)
		{
			ImGui::SetTooltip("cannot be deleted because this is currently the selected conveyor");
		}
		ImGui::SameLine();
		ImGui::SetNextItemWidth(100);
		ImGui::SliderFloat("width", &allConveyors[j].width, allConveyors[j].widthMin, allConveyors[j].widthMax);
		if (ImGui::CollapsingHeader("Attributes"))
		{
			std::vector<Attribute>& attributesList = allConveyors[j].attributes;
			if (ImGui::Button("new attribute")) attributesList.push_back(Attribute());
			std::vector<int> deletionsList;
			for (Attribute& a : attributesList)
			{

				ImGui::SetNextItemWidth(130);
				ImGui::PushID(&a);
				ImGui::InputText("##name", &a.name);
				bool name = ImGui::IsItemActive();
				ImGui::SameLine();
				ImGui::Text(":");
				ImGui::SameLine();
				ImGui::SetNextItemWidth(130);
				ImGui::InputText("##value", &a.value);
				bool value = ImGui::IsItemActive();
				settings->typing = name || value;
				if (a.isNumber && Tools::ContainsLetters(a.value))
				{
					a.value = "";
				}
				if (ImGui::Button("delete"))
				{
					int test = Tools::FindInList(attributesList, a);
					deletionsList.push_back(test);
				}
				ImGui::SameLine();
				ImGui::Checkbox("", &a.isNumber);
				if (ImGui::IsItemHovered()) ImGui::SetTooltip("check this when the value is a number");

				ImGui::PopID();
			}
			for (int i : deletionsList) Tools::DeleteFromList(attributesList, attributesList[deletionsList[i]]);

		}
		ImGui::PopID();
		ImGui::Separator();
	}
	ImGui::PopStyleColor();
	for (int i : deletions)
		DeleteFromList(currentLayer.allConveyors, currentLayer.allConveyors[i]);

}
