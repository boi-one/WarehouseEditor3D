#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>

#include "Settings.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Mouse.h"

#include <chrono>

void SDLEvents(SDL_Event& event, Settings& settings, Camera2D& camera, Mouse& mouse, float deltaTime, std::vector<Mesh>& allMeshes, glm::vec2& screen, bool& overUI);

int main()
{
	Settings settings;
	Camera2D camera({ 0.0f, 0.0f, 1.0f });
	Mouse mouse;
	std::vector<Mesh> allMeshes;

	auto start_time = std::chrono::high_resolution_clock::now();

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		return -1;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// Create SDL window
	SDL_Window* window = SDL_CreateWindow("Warehouse Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, camera.viewport.screenWidth, camera.viewport.screenHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (!window)
	{
		SDL_Quit();
		return -1;
	}

	// Create SDL GL context
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	if (!gl_context)
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> duration = end_time - start_time;
	std::cout << "SDL Initialization and Window Creation took " << duration.count() << " seconds." << std::endl;


	// Initialize OpenGL loader
	if (glewInit())
	{
		SDL_GL_DeleteContext(gl_context);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return -1;
	}

	const GLubyte* vendor = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);

	std::cout << "Vendor: " << vendor << "\nRenderer: " << renderer << std::endl;

	// Setup ImGui
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO();


	Shader shader("shader.vert", "shader.frag");

	float vertices[] =
	{
		100.5f,  100.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top right
		100.5f, -100.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
	   -100.5f, -100.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // bottom left
	   -100.5f,  100.5f, 0.0f,  1.0f, 1.0f, 1.0f
	};

	unsigned int indices[] =
	{
		0, 1, 3,
		2, 1, 3
	};

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	shader.use();

	bool wireframe = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;

	int width = camera.viewport.screenWidth;
	int height = camera.viewport.screenHeight;

	bool autoScreenResolution = true;
	glm::vec2 screen = {0, 0};
	bool overUI = false;

	// Main loop
	while (settings.appRunning)
	{
		float currentFrame = (float)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		SDL_Event event;
		SDLEvents(event, settings, camera, mouse, deltaTime, allMeshes, screen, overUI); //TODO: convert mouse to screen position

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Rendering
		int display_w, display_h;
		SDL_GetWindowSize(window, &display_w, &display_h);
		SDL_GL_GetDrawableSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClearColor(0.45f, 0.55f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		screen.x = display_w;
		screen.y = display_h;

		//order:
		// clear color
		// openGL
		//imgui code
		// imgui render
		//swap window
		shader.use();

		// 3d cam: glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)display_w / (float)display_h, 0.1f, 1000.0f);

		//de transform is de camera het is de rand om het scherm heen die word verplaatst met de camera.position
		glm::mat4 transform;
		if (autoScreenResolution)
			transform = glm::ortho(-(float)display_w / 2 + camera.position.x, (float)display_w / 2 + camera.position.x, -(float)display_h / 2 + camera.position.y, (float)display_h / 2 + camera.position.y, -100.0f, 100.0f);
		else
			transform = glm::ortho(-width + camera.position.x, width + camera.position.x, -height + camera.position.y, height + camera.position.y, -100.0f, 100.0f);
		shader.setMat4("transform", transform);

		glBindVertexArray(VAO);

		for (Mesh& m : allMeshes)
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m.position);
			shader.setMat4("model", model);
			//std::cout << m.position.x << " " << m.position.y << std::endl;
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		ImGui::Begin("test");

		overUI = ImGui::IsWindowHovered();

		ImGui::Text("FPS: %.0f", 1 / deltaTime);
		if (ImGui::Button("wireframe"))
		{
			wireframe = !wireframe;
			if (wireframe)
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			else
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		ImGui::SliderInt("w", &width, 0, display_w);
		ImGui::SliderInt("h", &height, 0, display_h);

		if (!autoScreenResolution && ImGui::Button("revert to current screen aspect"))
		{
			width = display_w;
			height = display_h;
		}
		if (ImGui::Button("auto"))
		{
			autoScreenResolution = !autoScreenResolution;
		}
		if (ImGui::SliderFloat("speed", &camera.movementSpeed, 0, 3000.0f));

		ImGui::End();

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

void SDLEvents(SDL_Event& event, Settings& settings, Camera2D& camera, Mouse& mouse, float deltaTime, std::vector<Mesh>& allMeshes, glm::vec2& screen, bool& overUI)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouse.pixelPosition.x = mouseX;
	mouse.pixelPosition.y = mouseY;
	mouse.ndcPosition.x = ((float)mouseX / camera.viewport.screenWidth) * 2 - 1;
	mouse.ndcPosition.y = ((float)mouseY / camera.viewport.screenHeight) * 2 - 1;
	mouse.position.x = (mouseX / (screen.x) - 0.5f)* screen.x + camera.position.x;
	mouse.position.y = (0.5f - mouseY / screen.y) * screen.y  + camera.position.y;


	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
		{
			settings.appRunning = false;
		}break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (overUI) break;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				std::cout << mouse.position.x << " " << mouse.position.y << " " << mouse.pixelPosition.x << " " << mouse.pixelPosition.y << " | " << mouse.ndcPosition.x << " " << mouse.ndcPosition.y << std::endl;
				
				Mesh m(glm::vec3(mouse.position.x, mouse.position.y, 0));
				allMeshes.push_back(m);
				
			}
		}break;
		}
	}

	const Uint8* key = SDL_GetKeyboardState(0);

	if (key[SDL_SCANCODE_W])
		camera.ProcessKeyboard(Up, deltaTime);
	if (key[SDL_SCANCODE_S])
		camera.ProcessKeyboard(Down, deltaTime);
	if (key[SDL_SCANCODE_A])
		camera.ProcessKeyboard(Left, deltaTime);
	if (key[SDL_SCANCODE_D])
		camera.ProcessKeyboard(Right, deltaTime);
}