#pragma once
#include <SDL.h>
#include "imgui_impl_sdl2.h"

#include "CameraManager.h"
#include "Mouse.h"
#include "Settings.h"
#include "Conveyor.h"

enum Keys
{
	TAB,
	ESC,
	SPACE,
	W,
	A,
	S,
	D,
	R,
	Z,
	LSHIFT,
};

class Input
{
public:
	struct Key
	{
		Keys id;
		SDL_Scancode SDLId;
		
	private:
		bool firstPress = false;
		bool pressed = false;

	public:
		Key(Keys id, SDL_Scancode SDLId) 
		{ 
			this->id = id; 
			this->SDLId = SDLId;
		};

		/// <summary>
		/// 
		/// </summary>
		/// <returns>true once when pressed</returns>
		bool Down();
		/// <summary>
		/// 
		/// </summary>
		/// <returns>true for the duration of holding the key</returns>
		bool Hold();
		/// <summary>
		/// isnt used yet
		/// </summary>
		/// <returns></returns>
		bool Up();
		void RegisterInput(const Uint8* key);
	};
	Mouse mouse;

private:
	float* deltaTime = 0;
	bool* orthoProjection = 0;
	Camera2D* camera2d = 0;
	Camera3D* camera3d = 0;
	Settings* settings = 0;
	
	std::vector<Key> keys =
	{
		Key(Keys::TAB, SDL_SCANCODE_TAB),
		Key(Keys::ESC, SDL_SCANCODE_ESCAPE),
		Key(Keys::SPACE, SDL_SCANCODE_SPACE),
		Key(Keys::W, SDL_SCANCODE_W),
		Key(Keys::A, SDL_SCANCODE_A),
		Key(Keys::S, SDL_SCANCODE_S),
		Key(Keys::D, SDL_SCANCODE_D),
		Key(Keys::R, SDL_SCANCODE_R),
		Key(Keys::Z, SDL_SCANCODE_Z),
		Key(Keys::LSHIFT, SDL_SCANCODE_LSHIFT),
	};

public:
	Input(float* deltaTime, bool* orthoProjection, Camera2D* camera2d, Camera3D* camera3d, Settings* settings)
	{
		this->deltaTime = deltaTime;
		this->orthoProjection = orthoProjection;
		this->camera2d = camera2d;
		this->camera3d = camera3d;
		this->settings = settings;
	}

	void SDLEvents();
	void UpdateMouse(Mouse& mouse);
	void ProcessInput();
	void Update();
};