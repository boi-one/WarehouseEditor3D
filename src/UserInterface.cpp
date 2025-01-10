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
		ImGui::Begin("Info");
		mouse->overUI = ImGui::IsWindowHovered();
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
		ImGui::SetWindowSize({ cameraManager->camera2d.viewport.cameraWidth / 4, cameraManager->camera2d.viewport.cameraWidth / 7 }, ImGuiCond_Once);
		glm::vec2 centerScreen(cameraManager->camera2d.viewport.cameraWidth / 2, cameraManager->camera2d.viewport.cameraHeight / 2);
		ImGui::SetWindowPos({ centerScreen.x - ImGui::GetWindowSize().x / 2, centerScreen.y - ImGui::GetWindowSize().y / 2 }, ImGuiCond_Once);
		mouse->overUI = ImGui::IsWindowHovered();

		if (ImGui::Button("Save"));
		ImGui::SameLine();
		if (ImGui::Button("Load"));
		ImGui::SameLine();
		ImGui::PushStyleColor(ImGuiCol_Button, {.75f, 0, 0, 1});
		if (ImGui::Button("Exit")) settings->appRunning = false;
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
}
