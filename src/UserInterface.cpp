#include "UserInterface.h"

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
	if (settings->showInfo)
	{
		ImGui::Begin("Info", 0, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		mouse->overUI = ImGui::IsWindowHovered();
		ImGui::SetWindowPos({ 10, 10 });
		ImGui::Text("- FPS: %.0f", 1 / deltaTime);
		ImGui::Text("- Mouse position: X %.0f, Y %.0f", mouse->position.x, mouse->position.y);
		ImGui::Text("- 2D Camera position: X %.0f, Y %.0f", cameraManager->camera2d.position.x, cameraManager->camera2d.position.y);
		ImGui::Text("- 3D Camera position: X %.0f, Y %.0f, Z %.0f", cameraManager->camera3d.position.x, cameraManager->camera3d.position.y, cameraManager->camera3d.position.z);
		ImGui::Text("- Screen resolution: X %0.f, Y %0.f", cameraManager->camera2d.viewport.cameraWidth, cameraManager->camera2d.viewport.cameraHeight);
		ImGui::End();
	}

	if (settings->openSettings)
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
		ImGui::Begin("Settings & Help", 0, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
		ImGui::SetWindowSize({ (float)cameraManager->camera2d.viewport.windowWidth / 4, (float)cameraManager->camera2d.viewport.windowWidth / 7 });
		glm::vec2 centerScreen((float)cameraManager->camera2d.viewport.windowWidth / 2, (float)cameraManager->camera2d.viewport.windowHeight / 2);
		ImGui::SetWindowPos({ centerScreen.x - ImGui::GetWindowSize().x / 2, centerScreen.y - ImGui::GetWindowSize().y / 2 });
		mouse->overUI = ImGui::IsWindowHovered();

		if (ImGui::Button("Save"));
		ImGui::SameLine();
		if (ImGui::Button("Load"));
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, { .75f, 0, 0, 1 });
		if (ImGui::Button("Quit")) settings->appRunning = false;
		ImGui::PopStyleColor();
		ImGui::Spacing();

		if (ImGui::CollapsingHeader("Settings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("[Visuals]");
			ImGui::Spacing();
			VisualSettings();
			ImGui::Spacing();
			ImGui::Text("[3D Camera]");
			ImGui::Spacing();
			Camera3DSettings();
			ImGui::Spacing();
		}

		if (ImGui::CollapsingHeader("Keybindings", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("[General Keybindings]");
			ImGui::Spacing();
			ImGui::Text("Esc        : Open settings menu");
			ImGui::Text("Tab        : Switch projection");
			ImGui::Text("I          : Show info");

			ImGui::Spacing();
			ImGui::Text("[2D Keybindings]");
			ImGui::Spacing();
			ImGui::Text("W          : Move up");
			ImGui::Text("A          : Move left");
			ImGui::Text("S          : Move down");
			ImGui::Text("D          : Move right");
			ImGui::Text("Z          : Unselect Conveyor");
			ImGui::Text("R          : Reset 2D camera position");
			ImGui::Text("Left Shift : Merge conveyors together");
			ImGui::Text("X          : Unselect point");

			ImGui::Spacing();
			ImGui::Text("[3D Keybindings]");
			ImGui::Spacing();
			ImGui::Text("W          : Move forward");
			ImGui::Text("A          : Strafe left");
			ImGui::Text("S          : Move backwards");
			ImGui::Text("D          : Strafe right");
			ImGui::Text("Left Shift : Move down");
			ImGui::Text("Space      : Move up");
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

void UserInterface::Camera3DSettings()
{
	float fovMin = 10.f;
	float fovMax = 130.f;
	ImGui::SliderFloat("FOV", &cameraManager->camera3d.fov, fovMin, fovMax);
	float sensMin = 0.01f;
	float sensMax = 1.f;
	ImGui::SliderFloat("Sensitivity", &mouse->sensitivity, sensMin, sensMax);
}

void UserInterface::Layers(LayerManager& layerManager)
{
	ImGui::Begin("Layers");
	mouse->overUI = ImGui::IsWindowHovered();
	std::vector<int> deletions;

	if (ImGui::Button("add layer"))
	{
		layerManager.AddLayer();
	}

	for (int i = 0; i < layerManager.allLayers.size(); i++)
	{
		std::vector<Layer>& allLayers = layerManager.allLayers;
		int multiply = 25;
		allLayers[i].depth = i * multiply;
		for (Conveyor& c : allLayers[i].allConveyors)
		{
			for (Point& p : c.path)
			{
				p.depth = i * multiply;
			}
		}

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
		if (ImGui::Button("move up") && i > 0)
		{
			Layer tempLayer = allLayers[i];
			Layer tempLayerPrev = allLayers[i - 1];
			allLayers[i - 1] = tempLayer;
			allLayers[i] = tempLayerPrev;
		}
		ImGui::SameLine();
		if (ImGui::Button("move down") && i < allLayers.size() - 1)
		{
			Layer tempLayer = allLayers[i];
			Layer tempLayerPrev = allLayers[i + 1];
			allLayers[i + 1] = tempLayer;
			allLayers[i] = tempLayerPrev;

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
			if(!allLayers[i].selected)
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
		ImGui::PushID(j);
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
		ImGui::PopID();
		if (ImGui::IsItemHovered() && allConveyors[j].selected)
		{
			ImGui::SetTooltip("cannot be deleted because this is currently the selected conveyor");
		}
		ImGui::Separator();
	}
	ImGui::PopStyleColor();
	for (int i : deletions)
		DeleteFromList(currentLayer.allConveyors, currentLayer.allConveyors[i]);
}
