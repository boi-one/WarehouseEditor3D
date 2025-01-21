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

/// <summary>
/// all the keys you can use, when adding extra keys, keep the order the same as in the keys vector in the input class (THIS IS IMPORTANT!)
/// </summary>
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
	LCTRL,
	DEL,
};

/// <summary>
/// handles all the input in the program
/// </summary>
class Input
{
public:
	/// <summary>
	/// a key on your keyboard
	/// </summary>
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
		/// <summary>
		/// checks if the key is pressed
		/// </summary>
		/// <param name="key">which key you want to check</param>
		void RegisterInput(const Uint8* key);
	};
	Mouse mouse;

private:
	CameraManager* cameraManager = 0;
	Settings* settings = 0;
	LayerManager* layerManager = 0;
	Grid* grid = 0;
	JsonSerialization* jsonSerialization = 0;

	/// <summary>
	/// the list of all the keys being used, this has to be the same order as in the enum (IMPORTANT!)
	/// </summary>
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
		Key(Keys::LCTRL, SDL_SCANCODE_LCTRL),
		Key(Keys::DEL, SDL_SCANCODE_DELETE),
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
	/// <summary>
	/// polls all of the SDL events like exiting the window
	/// </summary>
	void SDLEvents();
	/// <summary>
	/// updates the mouse so it has the correct position
	/// </summary>
	/// <param name="mouse"></param>
	void UpdateMouse(Mouse& mouse);
	/// <summary>
	/// read which keys you press and perform the according action
	/// </summary>
	void ProcessInput();
	/// <summary>
	/// update all of the input
	/// </summary>
	/// <param name="deltaTime">used for the camera movement so it is smooth on all computers</param>
	void Update(float deltaTime);
	/// <summary>
	/// the movement code
	/// </summary>
	/// <param name="deltaTime">used for the camera movement so it is smooth on all computers</param>
	void Movement(float deltaTime);
};