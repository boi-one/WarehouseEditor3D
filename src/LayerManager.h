#pragma once
#include "glm\glm.hpp"

#include "Conveyor.h"

class Layer
{
public:
	static inline int layerCount;
	int id;
	bool selected = false;
	bool hidden = false;
	std::vector<Conveyor> allConveyors;
	Conveyor* selectedConveyor = 0;
	static inline bool connecting = false;
	static inline glm::vec3 newLineEnd;
	/// <summary>
	/// depth is how high the layer is in 3D, the point class has also a depth, because of the near clip plane of the orthographic camera you can max have 20 layers but if you decrease it to something like -2000 it will display more layers
	/// </summary>
	float depth = 0;

public:
	Layer()
	{
		this->id = layerCount++;
	}

	void DrawConveyors(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, glm::vec3& color);

	void UnselectConveyors()
	{
		for (Conveyor& c : allConveyors)
		{
			c.selected = false;
			c.edit = false;
			c.selectedPoint = 0;
		}
	}
};

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
		UnselectEverything();
		selectedLayer = &allLayers.emplace_back(Layer());
		selectedLayer->selected = true;
	}
	void RemoveLayer()
	{

	}

	void UnselectEverything();



	void DrawLayers(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection);
};