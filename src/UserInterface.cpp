#include "UserInterface.h"

void UserInterface::NewImGuiFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
}

void UserInterface::InterfaceInteraction(float deltaTime)
{
	ImGui::Begin("test");
	mouse->overUI = ImGui::IsWindowHovered();
	ImGui::Text("FPS: %.0f", 1 / deltaTime);
	ImGui::Text("- Mouse position: X %.0f, Y %.0f", mouse->position.x, mouse->position.y);
	ImGui::Text("- Camera position: X %.0f, Y %.0f", cameraManager->camera2d.position.x, cameraManager->camera2d.position.y);
	ImGui::Text("- 3D Camera position: X %.0f, Y %.0f, Z %.0f", 
		cameraManager->camera3d.position.x, cameraManager->camera3d.position.y, cameraManager->camera3d.position.z);
	ImGui::Text("- Screen resolution: X %d, Y %d", cameraManager->camera2d.viewport.cameraWidth, cameraManager->camera2d.viewport.cameraHeight);
	if (ImGui::Button("wireframe"))
	{
		settings->wireframe = !settings->wireframe;
		if (settings->wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (ImGui::Button("show axes")) settings->showAxes = !settings->showAxes;
	const char* currentProjection = cameraManager->orthoProjection ? "ortho (2D)" : "perspective (3D)";
	if (ImGui::Button(currentProjection)) cameraManager->orthoProjection = !cameraManager->orthoProjection;

	ImGui::End();
}
