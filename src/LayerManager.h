#pragma once
#include "glm\glm.hpp"
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Conveyor.h"
#include "Tools.h"
using namespace Tools;
#include "BridgeConveyor.h"

/// <summary>
/// a layer of conveyors
/// </summary>
class Layer
{
public:
	static inline int layerCount = 0;
	bool selected = false;
	bool hidden = false;
	std::vector<Conveyor> allConveyors;
	Conveyor* selectedConveyor = nullptr;
	static inline bool connecting = false;
	static inline glm::vec3 newLineEnd;
	/// <summary>
	/// depth is how high the layer is in 3D, the point class has also a depth, because of the near clip plane of the orthographic camera you can max have 40 layers (near plane distance/25) but if you decrease it to something like -2000 it will display more layers
	/// </summary>
	float depth = 0;
	float width = 10;
	int id;

public:
	Layer()
	{
		this->id = layerCount++;
	}
	/// <summary>
	/// draws all the conveyors in the layer
	/// </summary>
	/// <param name="shader">the shader for the mesh</param>
	/// <param name="cube">the mesh the conveyors use to render</param>
	/// <param name="mouse">used for position of the new line</param>
	/// <param name="orthoProjection">used to check if orthographic projection is enabled</param>
	/// <param name="color">the color of the conveyor being rendered</param>
	/// <param name="gridSnap">if the mouse should snap to the grid</param>
	void DrawConveyors(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, glm::vec3& color, bool& gridSnap, bool& cast, glm::vec3& mousePos);
	/// <summary>
	/// sets all the related pointers to 0 and sets all the bools related to false
	/// </summary>
	void UnselectConveyors()
	{
		for (Conveyor& c : allConveyors)
		{
			c.selected = false; //error soms??? heeft te maken met de layer switching
			c.edit = false;
			c.selectedPoint = nullptr;
			for (Point& p : c.path) p.selected = false;
		}

		if (!selectedConveyor) return;
		selectedConveyor = nullptr;
	};
	/// <summary>
	/// returns the closest conveyor in the layer relative to the origin parameter
	/// </summary>
	/// <param name="origin">the position used to check which conveyor is closest to it</param>
	/// <returns>pointer of the closest conveyor</returns>
	Conveyor* ReturnClosestConveyor(glm::vec3& origin);
	/// <summary>
	/// returns the closest conveyor in the layer relative to the origin parameter, ignores the conveyor given as parameter, so it doesn't check for the closest conveyor if it's is the selected one
	/// </summary>
	/// <param name="origin"></param>
	/// <param name="selected"></param>
	/// <returns></returns>
	Conveyor* ReturnClosestConveyor(glm::vec3& origin, Conveyor& selected);
	/// <summary>
	/// start adding new lines to an already existing conveyor
	/// </summary>
	/// <param name="position">position of the mouse</param>
	/// <param name="orthoProjection">if orthograpic projection is true or false</param>
	/// <returns>if a conveyor could be edited, but the return value was mainly used for debug purposes</returns>
	bool EditConveyor(glm::vec3& position, bool& orthoProjection);
	/// <summary>
	/// 
	/// </summary>
	/// <returns>the id of this layer</returns>
	int ID() const { return id; };
	/// <summary>
	/// set depth of this layer so when viewing in 3D all the layers have a different depth position
	/// </summary>
	/// <param name="layer">position of the layer</param>
	void SetDepth(int layer);

	friend void to_json(json& j, const Layer& l);
};

void from_json(const json& j, Layer& l);

/// <summary>
/// manages all the layers inside the program
/// </summary>
class LayerManager
{
public:
	Shader* shader = nullptr;
	Mesh* cube = nullptr;
	Mouse* mouse = nullptr;
	std::vector<Layer> allLayers;
	std::vector<BridgeConveyor> allBridgeConveyors;
	Layer* selectedLayer = nullptr;

public:
	LayerManager()
	{
		selectedLayer = &allLayers.emplace_back(Layer());
		selectedLayer->selected = true;
	}

private:
	bool constructed = false;

public:
	/// <summary>
	/// used as construct because some values aren't ready yet when the layer manager is created
	/// </summary>
	/// <param name="shader">the shader used for settings transforms</param>
	/// <param name="cube">the mesh of the conveyors per layer</param>
	/// <param name="mouse">used for positions</param>
	void LateConstruct(Shader* shader, Mesh* cube, Mouse* mouse)
	{
		if (constructed) return;

		this->shader = shader;
		this->cube = cube;
		this->mouse = mouse;
	}
	/// <summary>
	/// adds a layer to the layer vector
	/// </summary>
	void AddLayer()
	{
		allLayers.emplace_back(Layer());
	}
	/// <summary>
	/// sets all the related pointers to 0 and sets all the bools related to false
	/// </summary>
	void UnselectEverything();

	Conveyor* FindClosestConveyorFromAll(glm::vec3 origin);

	/// <summary>
	/// draws every point per conveyor per layer in the layer manager
	/// </summary>
	/// <param name="shader">used for transforms</param>
	/// <param name="cube">used to render the conveyors</param>
	/// <param name="mouse">used for positions</param>
	/// <param name="orthoProjection">used to check which projection is being used</param>
	/// <param name="gridSnap">used to determine if the mouse should snap to the grid</param>
	void DrawLayers(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, bool& gridSnap, bool& cast, glm::vec3& mousePos);

	/// <summary>
	/// Updates the anchor points of the bridge conveyors so when something changes they still are connected to the right points
	/// </summary>
	/// <param name="bg"></param>
	void UpdateBridgeConveyors(BridgeConveyor& bg);
};