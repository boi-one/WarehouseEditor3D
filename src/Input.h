#pragma once
#include <SDL.h>
#include "Camera2D.h"
#include "Camera3D.h"
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
	LSHIFT,

};

struct Key
{
	Keys id;
	bool pressed = false;
	Key(Keys id) { this->id = id; };

};

class Input
{
	float* deltaTime = 0;
	bool* orthoProjection = 0;
	Camera2D* camera2d = 0;
	Camera3D* camera3d = 0;
	Settings* settings = 0;
	Key keys[9] =
	{
		Key(Keys::TAB),
		Key(Keys::ESC),
		Key(Keys::SPACE),
		Key(Keys::W),
		Key(Keys::A),
		Key(Keys::S),
		Key(Keys::D),
		Key(Keys::R),
		Key(Keys::LSHIFT),
	};


public:
	Input(float* deltaTime, bool* orthoProjection, Camera2D* camera2d, Camera3D* camera3d, Settings* settings)
	{
		this->deltaTime = deltaTime;
		this->camera2d = camera2d;
		this->camera3d = camera3d;
		this->settings = settings;
	}

	void KeyInput();
	void Update();
};