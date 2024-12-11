#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <GL/glew.h>
#include <math.h>

#include "Settings.h"
#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Mouse.h"

void SDLEvents(SDL_Event& event, Settings& settings, Camera2D& camera, Mouse& mouse, float deltaTime, std::vector<Mesh>& allMeshes, glm::vec2& screen, bool& overUI, GLuint& VBO, GLuint& VAO);
void UI(bool& overUI, bool& wireframe, float deltaTime, Mouse& mouse, Camera2D& camera, int display_w, int display_h);

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
	SDL_Window* window = SDL_CreateWindow("Warehouse Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, camera.viewport.screenWidth, camera.viewport.screenHeight, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_MAXIMIZED);
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

std::vector<GLfloat> ConvertToFloat(std::vector<glm::vec3> vertices)
{
	std::vector<GLfloat> fvertices;

	for (glm::vec3 v : vertices)
	{
		fvertices.push_back(v.x);
		fvertices.push_back(v.y);
		fvertices.push_back(v.z);
	}

	return fvertices;
}

std::vector<glm::vec3> CreateLine(glm::vec3 start, glm::vec3 end, float width)
{
	std::vector<glm::vec3> vertices;

	glm::vec3 direction = glm::normalize(end - start);
	glm::vec3 perpendicular = glm::vec3(-direction.y, direction.x, 0);
			
	glm::vec3 offset = perpendicular * width;

	//triangle 1
	vertices.push_back(start + offset);	vertices.push_back({ 1, 0, 0 });
	vertices.push_back(start - offset);	vertices.push_back({ 1, 0, 0 });

	vertices.push_back(end + offset);	vertices.push_back({ 1, 0, 0 });
	vertices.push_back(end - offset);   vertices.push_back({ 1, 0, 0 });

	return vertices;
}

int main()
{
	Camera2D camera({ 0.0f, 0.0f, 1.0f }); //z omhoog en y de diepte in 2d

	InitReturn r = WindowInitialization(camera);
	if (r.failed == -1) return -1;
	SDL_Window* window = r.window;
	SDL_GLContext& gl_context = r.gl_context;

	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init("#version 330");
	ImGuiIO& io = ImGui::GetIO();

	Settings settings;
	Mouse mouse;
	std::vector<Mesh> allMeshes;
	Shader shader("shader.vert", "shader.frag");
	shader.use();

	GLuint VAO, VBO, EBO;

	std::vector<GLfloat> squareVertices =
	{
		 1.0f,  1.0f, 0.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,  1.0f, 1.0f, 1.0f
	};
	GLuint squareIndices[6] =
	{
		0, 1, 2,
		0, 2, 3
	};

	GLuint indices[] = 
	{ 
		0, 3, 1, 
		3, 1, 2 
	};

	//std::vector<glm::vec3> vertices = CreateLine(glm::vec3(-2, -2, 0), glm::vec3(2, -2, 0), 1.f);
	//
	//std::vector<GLfloat> fvertices;
	//
	//fvertices = ConvertToFloat(vertices);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fvertices.size(), fvertices.data(), GL_STATIC_DRAW);
	//
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * squareVertices.size(), squareVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glBindVertexArray(0); 

	bool wireframe = false;
	float deltaTime = 0.0f;
	float lastFrame = 0.0f;
	glm::vec2 screen = { 0, 0 };
	bool overUI = false;

	// Main loop
	while (settings.appRunning)
	{
		float currentFrame = (float)SDL_GetTicks() / 1000;
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		SDL_Event event;
		SDLEvents(event, settings, camera, mouse, deltaTime, allMeshes, screen, overUI, VBO, VAO);

		// Start the ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		// Rendering
		SDL_GetWindowSize(window, &camera.viewport.screenWidth, &camera.viewport.screenHeight);
		SDL_GL_GetDrawableSize(window, &camera.viewport.screenWidth, &camera.viewport.screenHeight);
		glViewport(0, 0, camera.viewport.screenWidth, camera.viewport.screenHeight);
		glClearColor(0.45f, 0.55f, 1.00f, 1.00f);
		glClear(GL_COLOR_BUFFER_BIT);
		screen.x = camera.viewport.screenWidth;
		screen.y = camera.viewport.screenHeight;

		//order:
		// clear color
		// openGL code
		// imgui code
		// imgui render
		//swap window
		shader.use();

		// 3d cam: glm::mat4 projection = glm::perspective(glm::radians(camera.fov), (float)display_w / (float)display_h, 0.1f, 1000.0f);
		//de transform is de camera het is de rand om het scherm heen die word verplaatst met de camera.position
		
		camera.Update();
		camera.SetTransform(shader);

		for (Mesh& m : allMeshes) 
		{
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, m.position);
			model = glm::scale(model, m.scale);
			shader.setMat4("model", model);
			

			m.Draw(squareVertices, EBO, indices);
		}

		UI(overUI, wireframe, deltaTime, mouse, camera, camera.viewport.screenWidth, camera.viewport.screenHeight);

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

void UI(bool& overUI, bool& wireframe, float deltaTime, Mouse& mouse, Camera2D& camera, int display_w, int display_h)
{
	ImGui::Begin("test");
	overUI = ImGui::IsWindowHovered();
	ImGui::Text("FPS: %.0f", 1 / deltaTime);
	glm::vec3 mousePos = { camera.ToWorldPosition(mouse.position) , 0};
	ImGui::Text("Mouse position: X %.0f, Y %.0f", mousePos.x, mousePos.y);
	ImGui::Text("Camera position: X %.0f, Y %.0f", camera.position.x, camera.position.y);
	ImGui::Text("Screen resolution: X %d, Y %d", display_w, display_h);
	if (ImGui::Button("wireframe"))
	{
		wireframe = !wireframe;
		if (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	ImGui::End();
}

void SDLEvents(SDL_Event& event, Settings& settings, Camera2D& camera, Mouse& mouse, float deltaTime, std::vector<Mesh>& allMeshes, glm::vec2& screen, bool& overUI, GLuint& VBO, GLuint& VAO)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	
	//de raw sdl screen position in pixels
	mouse.position = glm::vec3(mouseX, mouseY, 0);

	//mouse.position.x = (mouseX / (screen.x) - 0.5f) * screen.x + camera.position.x;
	//mouse.position.y = (0.5f - mouseY / screen.y) * screen.y + camera.position.y;

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
			if (event.wheel.y < 0 && camera.zoom > 1) camera.zoom--;
			if (event.wheel.y > 0 && camera.zoom < 8) camera.zoom++;
		}break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (overUI) break;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				glm::vec2 worldPos = camera.ToWorldPosition({ mouse.position.x, mouse.position.y });
				Mesh m(VBO, VAO, {worldPos, 0});
				std::cout << "push back mesh : " << m.position.x << " " << m.position.y << m.position.z << std::endl;
				allMeshes.push_back(m);
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				//stretch cube
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
	if (key[SDL_SCANCODE_R])
		camera.position = glm::vec3(0, 0, 0);
}