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
		ImGui::Text("FPS: %.0f", 1 / deltaTime);
		ImGui::Text("- Mouse position: X %.0f, Y %.0f", mouse->position.x, mouse->position.y);
		ImGui::Text("- 2D Camera position: X %.0f, Y %.0f", cameraManager->camera2d.position.x, cameraManager->camera2d.position.y);
		ImGui::Text("- 3D Camera position: X %.0f, Y %.0f, Z %.0f", cameraManager->camera3d.position.x, cameraManager->camera3d.position.y, cameraManager->camera3d.position.z);
		ImGui::Text("- Screen resolution: X %d, Y %d", cameraManager->camera2d.viewport.cameraWidth, cameraManager->camera2d.viewport.cameraHeight);
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
			ImGui::Text("Z          : Unselect Conveyor");
			ImGui::Text("Tab        : Switch projection");
			ImGui::Text("I          : Show info");

			ImGui::Spacing();
			ImGui::Text("[2D Keybindings]");
			ImGui::Spacing();
			ImGui::Text("W          : Move up");
			ImGui::Text("A          : Move left");
			ImGui::Text("S          : Move down");
			ImGui::Text("D          : Move right");
			ImGui::Text("R          : Reset 2D camera position");

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

	if (ImGui::Button("+"))
	{
		layerManager.UnselectEverything();
		layerManager.AddLayer();
	}
	ImGui::SameLine();
	if (ImGui::Button("-"))
	{
		layerManager.UnselectEverything();
		layerManager.RemoveLayer();
	}

	for (int i = 0; i < layerManager.allLayers.size(); i++)
	{
		std::vector<Layer>& allLayers = layerManager.allLayers;
		ImGui::PushID(allLayers[i].id);
		char layerLabel[128] = {};
		snprintf(layerLabel, sizeof(layerLabel), "%d. Layer: %d\nconveyor amount: %d", i, (int)allLayers[i].id, (int)allLayers[i].allConveyors.size());
		if (allLayers[i].selected) ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0, .6f, 0, 1));
		
		allLayers[i].depth = i * 50;
		for (Conveyor& c : allLayers[i].allConveyors)
		{
			for (Point& p : c.path)
			{
				p.depth = i * 50;
				for (Point& c : p.connections)
					c.depth = i * 50;
			}
		}

		if (ImGui::CollapsingHeader(layerLabel))
		{
			for (int j = 0; j < allLayers[i].allConveyors.size(); j++)
			{
				std::vector<Conveyor>& allConveyors = allLayers[i].allConveyors;
				char conveyorLabel[128] = {};
				snprintf(conveyorLabel, sizeof(conveyorLabel), "%d. Conveyor: %d", j, (int)allConveyors[j].path.size());
				if (ImGui::CollapsingHeader(conveyorLabel))
				{
					for (Point& point : allConveyors[j].path)
					{
						char pointLabel[128] = {};
						snprintf(pointLabel, sizeof(layerLabel), "point: %d X: %0.f Y: %0.f", point.id, point.position.x, point.position.y);
						if (ImGui::Button(pointLabel))
						{
							cameraManager->camera2d.position = point.position;
						}
						for (Point& c : point.connections)
						{
							snprintf(pointLabel, sizeof(layerLabel), "point: %d X: %0.f Y: %0.f", c.id, c.position.x, c.position.y);
							if (ImGui::Button(pointLabel))
							{
								cameraManager->camera2d.position = c.position;
							}
						}
					}
				}
			}
		}
		if (allLayers[i].selected) ImGui::PopStyleColor();
		if (ImGui::Button("select"))
		{
			layerManager.UnselectEverything();
			allLayers[i].selected = true;
			layerManager.selectedLayer = &allLayers[i];
		}
		ImGui::SameLine();
		ImGui::Checkbox("hide layer", &allLayers[i].hidden);
		ImGui::PopID();
	}

	ImGui::End();
}
