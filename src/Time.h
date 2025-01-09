#pragma once
#include <SDL.h>

class Time
{
private:
	static inline float deltaTime = 0;
	static inline float currentFrame = 0;
	static inline float lastFrame = 0;
public:
	static float GetDeltaTime();
};