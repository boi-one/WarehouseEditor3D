#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <math.h>

#include "Settings.h"
#include "Shader.h"
#include "Camera2D.h"
#include "Camera3D.h"
#include "Mesh.h"
#include "Mouse.h"
#include "Conveyor.h"
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void SDLEvents(SDL_Event& event, Settings& settings, Camera2D& camera, Camera3D& camera3d, Mouse& mouse, float deltaTime, bool& overUI, GLuint& VBO, GLuint& VAO, Mesh& plane, bool& orthoProjection);
void UI(bool& overUI, bool& wireframe, float deltaTime, Mouse& mouse, Camera2D& camera, Camera3D& camera3d, int display_w, int display_h, bool& orthoProjection, bool& depthTest);

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
	Camera2D camera({ 0.0f, 0.0f, 1.0f }); //z omhoog en y de diepte in 2d
	Camera3D camera3d;
#pragma region setup
	InitReturn r = WindowInitialization(camera);
	if (r.failed == -1) return -1;
	SDL_Window* window = r.window;
	SDL_GLContext& gl_context = r.gl_context;

	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO();
#pragma endregion setup
#pragma region localvariables
	Settings settings;
	Mouse mouse;
	std::vector<Mesh> allMeshes;
	Shader shader("shader.vert", "shader.frag");
	shader.use();
	GLuint VAO, VBO, EBO;
	bool wireframe = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	bool overUI = false;
	bool orthoProjection = true;
	bool depthTest = true;
	Mesh plane;
	plane.CreatePlane();
	Mesh cube;
	cube.CreateCube();
#pragma endregion localvariables
	// Main loop
	while (settings.appRunning)
	{
		if (depthTest)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
		}

		float currentFrame = (float)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		SDL_Event event;
		SDLEvents(event, settings, camera, camera3d, mouse, deltaTime, overUI, VBO, VAO, plane, orthoProjection);

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Rendering
		SDL_GetWindowSize(window, &camera.viewport.windowWidth, &camera.viewport.windowHeight);
		SDL_GL_GetDrawableSize(window, &camera.viewport.windowWidth, &camera.viewport.windowHeight);
		glViewport(0, 0, camera.viewport.windowWidth, camera.viewport.windowHeight);
		if (orthoProjection)	glClearColor(0.45f, 0.55f, 1.00f, 1.00f);
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
		if (orthoProjection)
			camera.SetTransform(shader);
		else
		{
			glm::mat4 projection = glm::perspectiveRH_ZO(glm::radians(camera3d.fov), (float)camera.viewport.cameraWidth / (float)camera.viewport.cameraHeight, 0.1f, 1000.0f);
			
			shader.setMat4("projection", projection);

			glm::mat4 view = camera3d.GetViewMatrix();
			shader.setMat4("view", view);
		}
		mouse.Update(camera);

		glm::mat4 cubeModel = glm::mat4(1.0f);
		cubeModel = glm::translate(cubeModel, { 0, 0, 0 });
		cubeModel = glm::scale(cubeModel, { 10, 10, 10 });
		shader.setMat4("model", cubeModel);
		cube.Draw(shader);

		if (ConveyorManager::selectedConveyor)
		{
			ConveyorManager::DrawNewLine(ConveyorManager::selectedConveyor->selectedPoint->position, mouse.position, plane, shader);
		}

		for (int i = 0; i < 3; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			if (i == 0)
			{
				shader.setVec3("mColor", { 1, 0, 0 });
				model = glm::scale(model, { 900, 1, 1 });
				shader.setMat4("model", model);
			}
			if (i == 1)
			{
				shader.setVec3("mColor", { 0, 1, 0 });
				model = glm::scale(model, { 1, 900, 1 });
				shader.setMat4("model", model);
			}
			if (i == 2)
			{
				shader.setVec3("mColor", { 0, 0, 1 });
				model = glm::scale(model, { 1, 1, 900 });
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
				model = glm::scale(model, { 10, 10, 1 });
				shader.setMat4("model", model);

				if (ConveyorManager::selectedConveyor && ConveyorManager::selectedConveyor->selectedPoint && &p == ConveyorManager::selectedConveyor->selectedPoint)
				{
					color = { 1, 0, 0 };
				}

				shader.setVec3("mColor", color);
				plane.Draw(shader);
			}
		}

		UI(overUI, wireframe, deltaTime, mouse, camera, camera3d, camera.viewport.windowWidth, camera.viewport.windowHeight, orthoProjection, depthTest);

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

void UI(bool& overUI, bool& wireframe, float deltaTime, Mouse& mouse, Camera2D& camera, Camera3D& camera3d, int display_w, int display_h, bool& orthoProjection, bool& depthTest)
{
	ImGui::Begin("test");
	overUI = ImGui::IsWindowHovered();
	ImGui::Text("FPS: %.0f", 1 / deltaTime);
	ImGui::Text("- Mouse position: X %.0f, Y %.0f", mouse.position.x, mouse.position.y);
	ImGui::Text("- Camera position: X %.0f, Y %.0f", camera.position.x, camera.position.y);
	ImGui::Text("- 3D Camera position: X %.0f, Y %.0f, Z %.0f", camera3d.Position.x, camera3d.Position.y, camera3d.Position.z);
	ImGui::Text("- Screen resolution: X %d, Y %d", display_w, display_h);
	if (ImGui::Button("wireframe"))
	{
		wireframe = !wireframe;
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	const char* currentProjection = orthoProjection ? "ortho (2D)" : "perspective (3D)";
	if (ImGui::Button(currentProjection)) orthoProjection = !orthoProjection;
	const char* depthTestEnabled = depthTest ? "depth test enabled" : "depth test disabled";
	if (ImGui::Button(depthTestEnabled))
	{
		depthTest = !depthTest;
	}
	ImGui::End();
}

void SDLEvents(SDL_Event& event, Settings& settings, Camera2D& camera, Camera3D& camera3d, Mouse& mouse, float deltaTime, bool& overUI, GLuint& VBO, GLuint& VAO, Mesh& plane, bool& orthoProjection)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouse.SetScreenPosition(mouseX, mouseY);

	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
		{
			settings.appRunning = false;
		}break;
		case SDL_MOUSEWHEEL:
		{
			if (overUI) break;
			if (event.wheel.y < 0 && camera.zoom > 0.4f) camera.zoom -= 0.4f;
			if (event.wheel.y > 0 && camera.zoom < 8) camera.zoom += 0.4f;
		}break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (overUI) break;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (!ConveyorManager::selectedConveyor)
				{
					Conveyor c;
					c.mesh = &plane;
					c.path.push_back(glm::vec3(mouse.position.x, mouse.position.y, -1));
					c.selectedPoint = &c.path.at(c.path.size() - 1);
					ConveyorManager::allConveyors.push_back(c);
					ConveyorManager::selectedConveyor = &ConveyorManager::allConveyors.at(ConveyorManager::allConveyors.size() - 1);
					ConveyorManager::selectedConveyor->selectedPoint = &ConveyorManager::selectedConveyor->path.at(ConveyorManager::selectedConveyor->path.size() - 1);
				}
				else
				{
					ConveyorManager::selectedConveyor->path.push_back(glm::vec3(mouse.position.x, mouse.position.y, -1));
					ConveyorManager::selectedConveyor->selectedPoint = &ConveyorManager::selectedConveyor->path.at(ConveyorManager::selectedConveyor->path.size() - 1);
				}
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{//crashes
				ConveyorManager::selectedConveyor->selectedPoint = Conveyor::ClosestPoint(ConveyorManager::selectedConveyor->path, mouse.position);
			}
		}break;
		}
	}

	const Uint8* key = SDL_GetKeyboardState(0);

	if (key[SDL_SCANCODE_W])
	{
		if (orthoProjection) camera.ProcessKeyboard(Up, deltaTime);
		else camera3d.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (key[SDL_SCANCODE_S])
	{
		if (orthoProjection) camera.ProcessKeyboard(Down, deltaTime);
		else camera3d.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (key[SDL_SCANCODE_A])
	{
		if (orthoProjection) camera.ProcessKeyboard(Left, deltaTime);
		else camera3d.ProcessKeyboard(LEFT, deltaTime);
	}
	if (key[SDL_SCANCODE_D])
	{
		if (orthoProjection) camera.ProcessKeyboard(Right, deltaTime);
		else camera3d.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (key[SDL_SCANCODE_R])
		camera.position = glm::vec3(0, 0, 3);
	if (key[SDL_SCANCODE_ESCAPE])
		ConveyorManager::selectedConveyor = 0;
}