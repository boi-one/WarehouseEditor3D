#include "Time.h"

float Time::GetDeltaTime()
{
	currentFrame = (float)SDL_GetTicks() / 1000;
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	return deltaTime;
}
