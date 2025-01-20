#pragma once
#include <SDL.h>
#include "imgui_impl_sdl2.h"

#include "CameraManager.h"
#include "Mouse.h"
#include "Settings.h"
#include "LayerManager.h"
#include "Conveyor.h"
#include "Grid.h"
#include "JsonSerialization.h"

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
	I,
	X,
	L,
	G,
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
	CameraManager* cameraManager = 0;
	Settings* settings = 0;
	LayerManager* layerManager = 0;
	Grid* grid = 0;
	JsonSerialization* jsonSerialization = 0;

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
		Key(Keys::I, SDL_SCANCODE_I),
		Key(Keys::X, SDL_SCANCODE_X),
		Key(Keys::L, SDL_SCANCODE_L),
		Key(Keys::G, SDL_SCANCODE_G),
	};

public:
	Input(CameraManager* cameraManager, Settings* settings, LayerManager* layerManager, Grid* grid, JsonSerialization* jsonSerialization)
	{
		this->cameraManager = cameraManager;
		this->settings = settings;
		mouse.cameraManager = cameraManager;
		this->layerManager = layerManager;
		this->grid = grid;
		this->jsonSerialization = jsonSerialization;
	}

	void SDLEvents();
	void UpdateMouse(Mouse& mouse);
	void ProcessInput();
	void Update(float deltaTime);
	void Movement(float deltaTime);
};