#include "Input.h"

void Input::SDLEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		ImGui_ImplSDL2_ProcessEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
		{
			settings->appRunning = false;
		}break;
		case SDL_MOUSEWHEEL:
		{
			mouse.Scroll(event, *camera2d);
		}break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (mouse.overUI) break;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				Conveyor::Place(mouse.position);
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{//crashes
				ConveyorManager::selectedConveyor->selectedPoint = Conveyor::ClosestPoint(ConveyorManager::selectedConveyor->path, mouse.position);
			}
		}break;
		case SDL_MOUSEMOTION:
		{
			mouse.MouseMovement3D(event, *camera3d);
		}break;
		}
	}
}

void Input::UpdateMouse(Mouse& mouse)
{
	mouse.Update(*camera2d);
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouse.SetScreenPosition(mouseX, mouseY);
}

void Input::ProcessInput()
{
	const Uint8* key = SDL_GetKeyboardState(0);
	for (Key& k : keys)
		k.RegisterInput(key);
}

void Input::Update()
{
	SDLEvents();
	mouse.Update(*camera2d);
	ProcessInput();

	if (keys[W].Hold())
	{
		if (*orthoProjection) camera2d->ProcessKeyboard(Up, *deltaTime);
		else camera3d->ProcessKeyboard(FORWARD, *deltaTime);
	}
	if (keys[A].Hold())
	{
		if (*orthoProjection) camera2d->ProcessKeyboard(Left, *deltaTime);
		else camera3d->ProcessKeyboard(LEFT, *deltaTime);
	}
	if (keys[S].Hold())
	{
		if (*orthoProjection) camera2d->ProcessKeyboard(Down, *deltaTime);
		else camera3d->ProcessKeyboard(BACKWARD, *deltaTime);
	}
	if (keys[D].Hold())
	{
		if (*orthoProjection) camera2d->ProcessKeyboard(Right, *deltaTime);
		else camera3d->ProcessKeyboard(RIGHT, *deltaTime);
	}
	if (keys[SPACE].Hold() && !*orthoProjection)
	{
		camera3d->ProcessKeyboard(UP, *deltaTime);
	}
	if (keys[LSHIFT].Hold() && !*orthoProjection)
	{
		camera3d->ProcessKeyboard(DOWN, *deltaTime);
	}
	if (keys[R].Down() && *orthoProjection)
	{
		camera2d->position = glm::vec3(0, 0, 3);
	}
	if (keys[Z].Down())
	{
		ConveyorManager::selectedConveyor = 0;
	}
	if (keys[ESC].Down())
	{
		settings->openSettings = !settings->openSettings;
	}
	if (keys[TAB].Down())
	{
		*orthoProjection = !*orthoProjection;
	}
}


bool Input::Key::Down()
{
	return firstPress;
}

bool Input::Key::Hold()
{
	return pressed;
}

bool Input::Key::Up()
{
	return false;
}

void Input::Key::RegisterInput(const Uint8* key)
{
	if (key[SDLId])
	{

		if (!pressed) firstPress = true;
		else firstPress = false;
		pressed = true;
	}
	else
	{
		pressed = false;
		firstPress = false;
	}
}
