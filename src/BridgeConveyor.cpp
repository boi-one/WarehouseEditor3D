#include "BridgeConveyor.h"

void BridgeConveyor::Draw()
{
	mesh->DrawLine(*shader, glm::vec3(0.357f, 0.498f, 0.859f), start, end, width, false);
}
