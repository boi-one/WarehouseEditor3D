#include "Input.h"

void Input::KeyInput()
{
	const Uint8* key = SDL_GetKeyboardState(0);

	if (key[SDL_SCANCODE_TAB]) keys[Keys::TAB].pressed = true;
	else keys[Keys::TAB].pressed = false;
	
	if (key[SDL_SCANCODE_ESCAPE]) keys[Keys::ESC].pressed = true;
	else keys[Keys::ESC].pressed = false;

	if (key[SDL_SCANCODE_SPACE]) keys[Keys::SPACE].pressed = true;
	else keys[Keys::SPACE].pressed = false;

	if (key[SDL_SCANCODE_W]) keys[Keys::W].pressed = true;
	else if (key[SDL_SCANCODE_W]) keys[Keys::W].pressed = false;

	if (key[SDL_SCANCODE_A]) keys[Keys::A].pressed = true;
	else if (key[SDL_SCANCODE_A]) keys[Keys::A].pressed = false;

	if (key[SDL_SCANCODE_S]) keys[Keys::S].pressed = true;
	else if (key[SDL_SCANCODE_S]) keys[Keys::S].pressed = false;

	if (key[SDL_SCANCODE_D]) keys[Keys::D].pressed = true;
	else if (key[SDL_SCANCODE_D]) keys[Keys::D].pressed = false;
	
	if (key[SDL_SCANCODE_R]) keys[Keys::R].pressed = true;
	else if (!key[SDL_SCANCODE_R]) keys[Keys::R].pressed = false;

	if (key[SDL_SCANCODE_LSHIFT]) keys[Keys::LSHIFT].pressed = true;
	else if (!key[SDL_SCANCODE_LSHIFT]) keys[Keys::LSHIFT].pressed = false;
}

void Input::Update()
{
	if (keys[W].pressed)
	{
		std::cout << "w" << std::endl;
		if (orthoProjection) camera2d->ProcessKeyboard(Up, *deltaTime);
		else camera3d->ProcessKeyboard(FORWARD, *deltaTime);
	}
	if (keys[A].pressed)
	{
		std::cout << "s" << std::endl;
		if (orthoProjection) camera2d->ProcessKeyboard(Down, *deltaTime);
		else camera3d->ProcessKeyboard(BACKWARD, *deltaTime);
	}
	if (keys[S].pressed)
	{
		std::cout << "s" << std::endl;
		if (orthoProjection) camera2d->ProcessKeyboard(Left, *deltaTime);
		else camera3d->ProcessKeyboard(LEFT, *deltaTime);
	}
	if (keys[D].pressed)
	{
		std::cout << "d" << std::endl;
		if (orthoProjection) camera2d->ProcessKeyboard(Right, *deltaTime);
		else camera3d->ProcessKeyboard(RIGHT, *deltaTime);
	}
	//if (key[SDL_SCANCODE_SPACE]) camera3d->ProcessKeyboard(UP, *deltaTime);
	//if (key[SDL_SCANCODE_LSHIFT]) camera3d->ProcessKeyboard(DOWN, *deltaTime);
	//if (key[SDL_SCANCODE_R]) camera2d->position = glm::vec3(0, 0, 3);
	//if (key[SDL_SCANCODE_ESCAPE]) ConveyorManager::selectedConveyor = 0;
	//if (key[SDL_SCANCODE_ESCAPE]) settings->openSettings = !settings->openSettings;
}
