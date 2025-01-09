#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>

#include "Settings.h"
#include "Shader.h"
#include "CameraManager.h"
#include "Mesh.h"
#include "Mouse.h"
#include "Conveyor.h"
#include "Input.h"
#include "Time.h"

void UI(bool& overUI, bool& wireframe, float deltaTime, Mouse& mouse, Camera2D& camera, Camera3D& camera3d, int display_w, int display_h, bool& orthoProjection, bool& showAxes);

struct InitReturn
{
	int failed;
	SDL_Window* window;
	SDL_GLContext gl_context;
	InitReturn(int failed = 0, SDL_Window* window = 0)
	{
		this->failed = failed;
		this->window = window;
		this->gl_context = 0;
	}
};
static InitReturn WindowInitialization(Camera2D& camera)
{
	InitReturn r;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return { -1 };
	}
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_Window* window = SDL_CreateWindow("Warehouse Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, camera.viewport.windowWidth, camera.viewport.windowHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
	if (!window)
	{
		SDL_Quit();
		return { -1 };
	}
	SDL_SetWindowMinimumSize(window, 160 * 4, 90 * 4);
	r.gl_context = SDL_GL_CreateContext(window);
	if (!r.gl_context)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return { -1 };
	}
	if (glewInit())
	{
		SDL_GL_DeleteContext(r.gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return { -1 };
	}
	r.window = window;
	return r;
}

int main()
{
#pragma region localvariables
	Camera2D camera({ 0.0f, 0.0f, 1.0f }); //z omhoog en y de diepte in 2d
	Camera3D camera3d;
#pragma endregion localvariables
	
#pragma region setup
	InitReturn r = WindowInitialization(camera);
	if (r.failed == -1) return -1;
	SDL_Window* window = r.window;
	SDL_GLContext& gl_context = r.gl_context;

	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO();
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
#pragma endregion setup

	Settings settings;
	std::vector<Mesh> allMeshes;
	Shader shader("shader.vert", "shader.frag");
	shader.use();
	GLuint VAO, VBO, EBO;
	bool wireframe = false;
	float dT = 0.0f;
	float lastFrame = 0.0f;
	bool overUI = false;
	bool showAxes = true;
	Mesh cube;
	Input input(&dT, &CameraManager::orthoProjection, &camera, &camera3d, &settings);
	cube.CreateCube();
	// Main loop
	while (settings.appRunning)
	{
		float deltaTime = Time::GetDeltaTime();
		dT = deltaTime;
		//SDLEvents(event, window, settings, camera, camera3d, mouse, deltaTime, overUI, VBO, VAO, cube, orthoProjection);
		input.Update();

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Rendering
		SDL_GetWindowSize(window, &camera.viewport.windowWidth, &camera.viewport.windowHeight);
		SDL_GL_GetDrawableSize(window, &camera.viewport.windowWidth, &camera.viewport.windowHeight);
		glViewport(0, 0, camera.viewport.windowWidth, camera.viewport.windowHeight);
		if (CameraManager::orthoProjection)	glClearColor(0.45f, 0.55f, 1.00f, 1.00f);
		else glClearColor(0.25f, 0.25f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		//order:
		// clear color
		// openGL code
		// imgui code
		// imgui render
		//swap window
		shader.use();
		camera.Update();

		if (CameraManager::orthoProjection)
		{
			camera.SetTransform(shader);
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
		else
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);

			glm::mat4 projection = glm::perspectiveRH_NO(glm::radians(camera3d.fov), (float)camera.viewport.cameraWidth / (float)camera.viewport.cameraHeight, 0.1f, 10000.0f);
			
			shader.setMat4("projection", projection);

			glm::mat4 view = camera3d.GetViewMatrix();
			shader.setMat4("view", view);
		}


		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, { 0, 0, 0 });
		cubeModel = glm::scale(cubeModel, { 2, 2, 2 });
		shader.setMat4("model", cubeModel);
		shader.setVec3("mColor", { 1, 1, 1 });
		cube.Draw(shader);
		shader.setVec3("mColor", { 1, 0, 0 });

		if (ConveyorManager::selectedConveyor)
		{
			ConveyorManager::DrawNewLine(ConveyorManager::selectedConveyor->selectedPoint->position, input.mouse.position, cube, shader);
		}

		int axes = 0;
		if (showAxes) axes = 3;
		for (int i = 0; i < axes; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			if (i == 0)
			{
				shader.setVec3("mColor", { 1, 0, 0 });
				model = glm::scale(model, { 9000, 1, 1 });
				shader.setMat4("model", model);
			}
			if (i == 1)
			{
				shader.setVec3("mColor", { 0, 1, 0 });
				model = glm::scale(model, { 1, 9000, 1 });
				shader.setMat4("model", model);
			}
			if (i == 2)
			{
				shader.setVec3("mColor", { 0, 0, 1 });
				model = glm::scale(model, { 1, 1, 9000 });
				shader.setMat4("model", model);
			}
			
			cube.Draw(shader);
		}
		shader.setVec3("mColor", { 1, 1, 1 });

		for (Conveyor& c : ConveyorManager::allConveyors) //draw lines
		{
			glm::vec3 color = { 1, 1, 1 };
			shader.setVec3("mColor", color);
			for (int i = 0; i < c.path.size() - 1; i++)
			{
				c.DrawLine(c.path.at(i).position, c.path.at(i + 1).position, shader);
			}
		}
		for (Conveyor& c : ConveyorManager::allConveyors)
		{
			for (Point& p : c.path) //draw points
			{
				glm::vec3 color = { 1, 1, 1 };

				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, p.position);
				model = glm::scale(model, { 11, 11, 11 });
				shader.setMat4("model", model);

				if (ConveyorManager::selectedConveyor && ConveyorManager::selectedConveyor->selectedPoint && &p == ConveyorManager::selectedConveyor->selectedPoint)
				{
					color = { 1, 0, 0 };
				}

				shader.setVec3("mColor", color);
				cube.Draw(shader);
			}
		}

		UI(overUI, wireframe, deltaTime, input.mouse, camera, camera3d, camera.viewport.windowWidth, camera.viewport.windowHeight, CameraManager::orthoProjection, showAxes);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

void UI(bool& overUI, bool& wireframe, float deltaTime, Mouse& mouse, Camera2D& camera, Camera3D& camera3d, int display_w, int display_h, bool& orthoProjection, bool& showAxes)
{
	ImGui::Begin("test");
	overUI = ImGui::IsWindowHovered();
	ImGui::Text("FPS: %.0f", 1 / deltaTime);
	ImGui::Text("- Mouse position: X %.0f, Y %.0f", mouse.position.x, mouse.position.y);
	ImGui::Text("- Camera position: X %.0f, Y %.0f", camera.position.x, camera.position.y);
	ImGui::Text("- 3D Camera position: X %.0f, Y %.0f, Z %.0f", camera3d.position.x, camera3d.position.y, camera3d.position.z);
	ImGui::Text("- Screen resolution: X %d, Y %d", display_w, display_h);
	if (ImGui::Button("wireframe"))
	{
		wireframe = !wireframe;
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	if (ImGui::Button("show axes")) showAxes = !showAxes;
	const char* currentProjection = orthoProjection ? "ortho (2D)" : "perspective (3D)";
	if (ImGui::Button(currentProjection)) orthoProjection = !orthoProjection;

	ImGui::End();
}