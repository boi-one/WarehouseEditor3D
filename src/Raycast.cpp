#include "Raycast.h"

void Raycast::Ray()
{
	glm::vec2 ndc;
	ndc.x = (2.f * mouse->screenPosition.x) / viewport->windowWidth - 1.f;
	ndc.y = 1.f - (2.f * mouse->screenPosition.y) / viewport->windowHeight;

	glm::vec4 rayClip(ndc.x, ndc.y, -1.f, 1.f);
	
	glm::vec4 rayEye = glm::inverse(cameraManager->camera3d.projection) * rayClip;
	rayEye = glm::vec4(rayEye.x, rayEye.y, -1.f, 0.f);
	
	glm::vec3 rayWorld = glm::inverse(cameraManager->camera3d.GetViewMatrix()) * rayEye;
	rayWorld = glm::normalize(rayWorld);

	origin = cameraManager->camera3d.position;
	direction = rayWorld;
}

bool Raycast::Intersection(float planeHeight)
{
	if (glm::abs(direction.z) < 0.0000001f) return false;

	float t = (planeHeight - origin.z) / direction.z;

	if (t < 0) return false;

	intersection = origin + t * direction;

	return true;
}
