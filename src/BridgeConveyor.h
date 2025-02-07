#pragma once
#include "Mesh.h"
#include "Conveyor.h"
#include "Shader.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

/// <summary>
/// used to connect conveyors from multiple layers to eachother
/// </summary>
class BridgeConveyor 
{
public:
	int startPointID = 0;
	Point* startPoint = nullptr;
	int endPointID = 0;
	Point* endPoint = nullptr;
	float width = 0;
	Mesh* mesh = nullptr;
	Shader* shader = nullptr;

	BridgeConveyor() = default;

	BridgeConveyor(Point* startPoint, Point* endPoint, float width, Mesh* mesh, Shader* shader)
	{
		this->startPoint = startPoint;
		this->startPointID = startPoint->id;
		this->endPoint = endPoint;
		this->endPointID = endPoint->id;
		this->width = width;
		this->mesh = mesh;
		this->shader = shader;
	}
	/// <summary>
	/// draws the conveyor
	/// </summary>
	void Draw();

	friend void to_json(json& j, const BridgeConveyor& bg);
};

void from_json(const json& j, BridgeConveyor& bg);