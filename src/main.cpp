
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
#include "UserInterface.h"
#include "Grid.h"

/// <summary>
/// used to return multiple values needed for windowInitialization
/// </summary>
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

/// <summary>
/// Initialize the SDL window
/// </summary>
/// <param name="camera">used for the width and height of the window</param>
/// <returns>the SDL window and gl_context </returns>
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
	CameraManager cameraManager;
	float currentFrame = 0;
	float lastFrame = 0;
	float deltaTime = 0;

#pragma region setup
	InitReturn r = WindowInitialization(cameraManager.camera2d);
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

	Shader shader("shader.vert", "shader.frag");
	shader.use();

	Settings settings;
	LayerManager layerManager;
	Grid grid;
	JsonSerialization jsonSerialization;
	Input input(&cameraManager, &settings, &layerManager, &grid, &jsonSerialization);
	UserInterface ui(&settings, &input.mouse, &cameraManager, &layerManager, &jsonSerialization);
	Mesh cube;
	Conveyor::mesh = &cube;
	jsonSerialization.LateConstruct(&cube);
	layerManager.LateConstruct(&shader, &cube, &input.mouse);
	// Main loop
	while (settings.appRunning)
	{
		currentFrame = (float)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		input.Update(deltaTime);
		ui.NewImGuiFrame();

		// Rendering
		SDL_GetWindowSize(window, &cameraManager.camera2d.viewport.windowWidth, &cameraManager.camera2d.viewport.windowHeight);
		SDL_GL_GetDrawableSize(window, &cameraManager.camera2d.viewport.windowWidth, &cameraManager.camera2d.viewport.windowHeight);
		glViewport(0, 0, cameraManager.camera2d.viewport.windowWidth, cameraManager.camera2d.viewport.windowHeight);
		if (cameraManager.orthoProjection)	glClearColor(0.45f, 0.55f, 1.00f, 1.00f);
		else glClearColor(0.25f, 0.25f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_DEPTH_BUFFER_BIT);
		//order: clear color -> openGL code -> imgui code -> imgui render -> swap window
		shader.use();
		cameraManager.camera2d.Update();
		cameraManager.UpdateProjection(shader, settings.openSettings);
		cube.RenderAxis(shader, settings.showAxes);
		grid.Draw(cube, shader, settings.showGrid);
		layerManager.DrawLayers(shader, cube, input.mouse, cameraManager.orthoProjection, settings.gridSnap);
		ui.InterfaceInteraction(deltaTime);

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