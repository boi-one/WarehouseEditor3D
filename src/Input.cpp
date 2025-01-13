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
			if (settings->openSettings) break;
			mouse.Scroll(event, cameraManager->camera2d);
		}break;
		case SDL_MOUSEBUTTONDOWN:
		{
			if (mouse.overUI) break;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				if (settings->openSettings || !cameraManager->orthoProjection) break;
				if (!layerManager->selectedLayer->selectedConveyor)
				{
					layerManager->selectedLayer->selectedConveyor = &layerManager->selectedLayer->allConveyors.emplace_back(Conveyor());
					layerManager->selectedLayer->selectedConveyor->selected = true;
				}
				layerManager->selectedLayer->selectedConveyor->Place(mouse.position);
			}
			if (event.button.button == SDL_BUTTON_RIGHT)
			{//crashes
				if (!layerManager->selectedLayer || !layerManager->selectedLayer->selectedConveyor) break;
				layerManager->selectedLayer->selectedConveyor->selectedPoint = 
					layerManager->selectedLayer->selectedConveyor->ClosestPoint(layerManager->selectedLayer->selectedConveyor->path, mouse.position);
			}
		}break;
		case SDL_MOUSEMOTION:
		{
			if (settings->openSettings)
			{
				ConveyorManager::selectedConveyor = 0;
				break;
			}
			mouse.MouseMovement3D(event, cameraManager->camera3d);
		}break;
		}
	}
}

void Input::UpdateMouse(Mouse& mouse)
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	mouse.SetScreenPosition(mouseX, mouseY);
	mouse.Update(cameraManager->camera2d);
}

void Input::ProcessInput()
{
	const Uint8* key = SDL_GetKeyboardState(0);
	for (Key& k : keys)
		k.RegisterInput(key);
}

void Input::Update(float deltaTime)
{
	SDLEvents();
	UpdateMouse(mouse);
	ProcessInput();

	if (!settings->openSettings) Movement(deltaTime);

	if (keys[R].Down() && cameraManager->orthoProjection)
	{
		cameraManager->camera2d.position = glm::vec3(0, 0, 3);
	}
	if (keys[Z].Down())
	{
		if (!layerManager->selectedLayer->selectedConveyor) return;
		layerManager->selectedLayer->selectedConveyor->selected = false;
		layerManager->selectedLayer->selectedConveyor = 0;
		layerManager->selectedLayer->UnselectConveyors();
	}
	if (keys[ESC].Down())
	{
		settings->openSettings = !settings->openSettings;
	}
	if (keys[TAB].Down())
	{
		cameraManager->orthoProjection = !cameraManager->orthoProjection;
	}
	if (keys[I].Down())
	{
		settings->showInfo = !settings->showInfo;
	}
}

void Input::Movement(float deltaTime)
{
	if (keys[W].Hold())
	{
		if (cameraManager->orthoProjection) cameraManager->camera2d.ProcessKeyboard(Up, deltaTime);
		else cameraManager->camera3d.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (keys[A].Hold())
	{
		if (cameraManager->orthoProjection) cameraManager->camera2d.ProcessKeyboard(Left, deltaTime);
		else cameraManager->camera3d.ProcessKeyboard(LEFT, deltaTime);
	}
	if (keys[S].Hold())
	{
		if (cameraManager->orthoProjection) cameraManager->camera2d.ProcessKeyboard(Down, deltaTime);
		else cameraManager->camera3d.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (keys[D].Hold())
	{
		if (cameraManager->orthoProjection) cameraManager->camera2d.ProcessKeyboard(Right, deltaTime);
		else cameraManager->camera3d.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (keys[SPACE].Hold() && !cameraManager->orthoProjection)
	{
		cameraManager->camera3d.ProcessKeyboard(UP, deltaTime);
	}
	if (keys[LSHIFT].Hold() && !cameraManager->orthoProjection)
	{
		cameraManager->camera3d.ProcessKeyboard(DOWN, deltaTime);
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
