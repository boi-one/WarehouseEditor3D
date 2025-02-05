#include "BridgeConveyor.h"

void BridgeConveyor::Draw()
{
	if (!startPoint || !endPoint) return;
	mesh->DrawLine(*shader, glm::vec3(0.357f, 0.498f, 0.859f), startPoint->position, endPoint->position, width, false);
}

void to_json(json& j, const BridgeConveyor& bg)
{
	j = json
	{
		{"startpointid", bg.startPointID},
		{"endpointid", bg.endPointID},
		{"width", bg.width},
	};
}

void from_json(const json& j, BridgeConveyor& bg)
{
	bg.startPointID = j.at("startpointid").get<int>();
	bg.endPointID = j.at("endpointid").get<int>();
	bg.width = j.at("width").get<float>();
}
