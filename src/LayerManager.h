#pragma once
#include "glm\glm.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Conveyor.h"
#include "Tools.h"
using namespace Tools;

class Layer
{
public:
	static inline int layerCount = 0;
	bool selected = false;
	bool hidden = false;
	std::vector<Conveyor> allConveyors;
	Conveyor* selectedConveyor = 0;
	static inline bool connecting = false;
	static inline glm::vec3 newLineEnd;
	/// <summary>
	/// depth is how high the layer is in 3D, the point class has also a depth, because of the near clip plane of the orthographic camera you can max have 40 layers (near plane distance/25) but if you decrease it to something like -2000 it will display more layers
	/// </summary>
	float depth = 0;
	int id;

public:
	Layer()
	{
		this->id = layerCount++;
	}

	void DrawConveyors(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, glm::vec3& color, bool& gridSnap);
	void UnselectConveyors()
	{
		for (Conveyor& c : allConveyors)
		{
			c.selected = false; //error soms??? heeft te maken met de layer switching
			c.edit = false;
			c.selectedPoint = 0;
		}

		if (!selectedConveyor) return;
		selectedConveyor = 0;
	};
	Conveyor* ReturnClosestConveyor(glm::vec3& origin);
	Conveyor* ReturnClosestConveyor(glm::vec3& origin, Conveyor& selected);
	bool EditConveyor(glm::vec3& position, bool& orthoProjection);
	int ID() const { return id; };
	void SetDepth(int layer);

	friend void to_json(json& j, const Layer& l);
};

void from_json(const json& j, Layer& l);

class LayerManager
{
public:
	Shader* shader = 0;
	Mesh* cube = 0;
	Mouse* mouse = 0;
	std::vector<Layer> allLayers;
	Layer* selectedLayer = 0;

public:
	LayerManager()
	{
		selectedLayer = &allLayers.emplace_back(Layer());
		selectedLayer->selected = true;
	}

private:
	bool constructed = false;

public:
	void LateConstruct(Shader* shader, Mesh* cube, Mouse* mouse)
	{
		if (constructed) return;

		this->shader = shader;
		this->cube = cube;
		this->mouse = mouse;
	}

	void AddLayer()
	{
		allLayers.emplace_back(Layer());
	}

	void UnselectEverything();

	void DrawLayers(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, bool& gridSnap);

	friend void to_json(json& j, const LayerManager& lm);
};

void from_json(const json& j, LayerManager& lm);