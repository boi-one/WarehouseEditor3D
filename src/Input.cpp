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
			settings->typing = false;
			if (mouse.overUI) break;
			if (event.button.button == SDL_BUTTON_LEFT)
			{
				glm::vec3 position = { 0, 0, 0 };
				if (cameraManager->orthoProjection)
					position = settings->gridSnap ? mouse.gridPosition : mouse.position;
				else position = cameraManager->camera3d.raycastIntersection;

				if (!layerManager->selectedLayer->selectedConveyor)
				{
					layerManager->selectedLayer->selectedConveyor = &layerManager->selectedLayer->allConveyors.emplace_back(Conveyor(layerManager->selectedLayer->width));
					layerManager->selectedLayer->selectedConveyor->selected = true;
					layerManager->selectedLayer->selectedConveyor->edit = true;
				}
				if (layerManager->selectedLayer->selected && layerManager->selectedLayer->selectedConveyor && layerManager->selectedLayer->selectedConveyor->edit)
					layerManager->selectedLayer->selectedConveyor->NewPoint(position);
			}

			if (event.button.button == SDL_BUTTON_RIGHT)
			{
				glm::vec3 position = { 0, 0, 0 };
				if (cameraManager->orthoProjection)
					position = settings->gridSnap ? mouse.gridPosition : mouse.position;
				else position = cameraManager->camera3d.raycastIntersection;

				if (!layerManager->selectedLayer || layerManager->selectedLayer->allConveyors.size() < 1) break;
				if (!layerManager->selectedLayer->selectedConveyor)
				{
					layerManager->selectedLayer->selectedConveyor = layerManager->selectedLayer->ReturnClosestConveyor(position);
					if (!layerManager->selectedLayer->selectedConveyor) break;
					layerManager->selectedLayer->selectedConveyor->selected = true;
					layerManager->selectedLayer->selectedConveyor->edit = true;
					break;
				}
				if (!layerManager->selectedLayer->selectedConveyor->edit) layerManager->selectedLayer->selectedConveyor->edit = true;
				Conveyor& selectedConveyor = *layerManager->selectedLayer->selectedConveyor;
				layerManager->selectedLayer->selectedConveyor->selectedPoint = selectedConveyor.ClosestPoint(position, 99999);
				for (Point& p : layerManager->selectedLayer->selectedConveyor->path) p.selected = false;
				layerManager->selectedLayer->selectedConveyor->selectedPoint->selected = true;
				layerManager->selectedLayer->selectedConveyor->selected = true;
			}

			if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				mouse.middleMouseFirstPress = true;
			}
		}break;
		case SDL_MOUSEBUTTONUP:
		{
			if (event.button.button == SDL_BUTTON_MIDDLE)
			{
				mouse.dragOffset = glm::vec3(0, 0, 0);
				mouse.middleMouseFirstPress = false;
				mouse.middleMousePressed = false;
			}
		}break;
		case SDL_MOUSEMOTION:
		{
			if (!settings->openSettings)
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
	mouse.Update(cameraManager->camera2d, grid->cellSize);
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

	if (keys[ESC].Down())
	{
		settings->openSettings = !settings->openSettings;
		if (!settings->openSettings)
			jsonSerialization->SerializeMouse(mouse.sensitivity, cameraManager->camera3d.fov);
	}

	if (settings->typing) return;

	if (!settings->openSettings) Movement(deltaTime);

	if (keys[R].Down())
	{
		if (cameraManager->orthoProjection)	cameraManager->camera2d.position = glm::vec3(0, 0, 3);
		else cameraManager->camera3d.position = glm::vec3(-100.0f, 0.0f, 100.0f);
	}
	if (keys[Z].Down())
	{
		layerManager->selectedLayer->UnselectConveyors();
	}
	if (keys[TAB].Down())
	{
		cameraManager->orthoProjection = !cameraManager->orthoProjection;
	}
	if (keys[I].Down())
	{
		settings->showInfo = !settings->showInfo;
	}
	if (keys[LSHIFT].Down())
	{
		layerManager->selectedLayer->EditConveyor(mouse.position, cameraManager->orthoProjection);
	}
	if (keys[X].Down())
	{
		if (layerManager->selectedLayer->selectedConveyor)
			layerManager->selectedLayer->selectedConveyor->edit = false;
	}
	if (keys[L].Down())
	{
		settings->showLayers = !settings->showLayers;
	}
	if (keys[G].Down())
	{
		settings->gridSnap = !settings->gridSnap;
	}
	if (keys[DEL].Down() && keys[LALT].Hold() && layerManager->selectedLayer->selectedConveyor)
	{
		Tools::DeleteFromList(layerManager->selectedLayer->allConveyors, *layerManager->selectedLayer->selectedConveyor);
		layerManager->selectedLayer->UnselectConveyors();
	}
	if (keys[DEL].Down() && layerManager->selectedLayer->selectedConveyor)
	{
		Point* deletePoint = 0;
		for (Point& p : layerManager->selectedLayer->selectedConveyor->path)
		{
			if (p.selected)
			{
				deletePoint = &p;
				break;
			}
		}
		for (Point& p : layerManager->selectedLayer->selectedConveyor->path)
		{
			if (!deletePoint) break;

			if (Tools::ContainsInList(p.connections, *deletePoint))
			{
				for (Point& cp : deletePoint->connections)
					p.connections.push_back(cp);
				Tools::DeleteFromList(p.connections, *deletePoint);
				layerManager->selectedLayer->selectedConveyor->selectedPoint = &p;
			}
		}
		Tools::DeleteNonIdenticalFromList(layerManager->selectedLayer->selectedConveyor->path, *deletePoint);
		if (layerManager->selectedLayer->selectedConveyor->path.size() < 1)
		{
			Tools::DeleteFromList(layerManager->selectedLayer->allConveyors, *layerManager->selectedLayer->selectedConveyor);
			layerManager->selectedLayer->UnselectConveyors();
		}
	}
	if (keys[ALEFT].Hold())
	{
		layerManager->selectedLayer->selectedConveyor->Rotate(-1);
	}
	if (keys[ARIGHT].Hold())
	{
		layerManager->selectedLayer->selectedConveyor->Rotate(1);
	}

	if (mouse.middleMouseFirstPress)
	{
		glm::vec3 position = { 0, 0, 0 };
		if (cameraManager->orthoProjection)
			position = mouse.position;
		else
			position = cameraManager->camera3d.raycastIntersection;
		mouse.dragOffset = position;
		mouse.middleMouseFirstPress = false;
		mouse.middleMousePressed = true;
	}
	if (mouse.middleMousePressed)
	{
		glm::vec3 position = { 0, 0, 0 };
		if (cameraManager->orthoProjection)
			position = mouse.position;
		else position = cameraManager->camera3d.raycastIntersection;
		if (layerManager->selectedLayer->selectedConveyor && layerManager->selectedLayer->selectedConveyor->selected)
		{
			glm::vec3 difference = glm::vec3(position.x - mouse.dragOffset.x, position.y - mouse.dragOffset.y, layerManager->selectedLayer->depth);

			for (Point& basePoint : layerManager->selectedLayer->selectedConveyor->path)
			{
				basePoint.position.x += difference.x;
				basePoint.position.y += difference.y;

				for (Point& p : basePoint.connections)
				{
					p.position.x += difference.x;
					p.position.y += difference.y;
				}
			}

			mouse.dragOffset = position;
		}
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
	if (keys[LCTRL].Hold())
	{
		if (cameraManager->orthoProjection)	cameraManager->camera2d.movementSpeed = cameraManager->camera2d.baseMovementSpeed * 2;
		else cameraManager->camera3d.movementSpeed = cameraManager->camera3d.baseMovementSpeed * 20;
	}
	else
	{
		if (cameraManager->orthoProjection) cameraManager->camera2d.movementSpeed = cameraManager->camera2d.baseMovementSpeed;
		else cameraManager->camera3d.movementSpeed = cameraManager->camera3d.baseMovementSpeed;
	}
	if (keys[C].Down()) cameraManager->camera3d.cast = !cameraManager->camera3d.cast;
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
