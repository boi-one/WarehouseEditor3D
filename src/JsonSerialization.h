#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

#include "LayerManager.h"

/// <summary>
/// saves and loads a scene so you can come back later to edit it
/// </summary>
class JsonSerialization
{
public:
	Mesh* cube = 0;
public:
	/// <summary>
	/// used because not all the values are initialized when JsonSerialization is created
	/// </summary>
	/// <param name="cube">a pointer to the mesh being used for the conveyors</param>
	void LateConstruct(Mesh* cube)
	{
		this->cube = cube;
	}
	/// <summary>
	/// saves the existing layers
	/// </summary>
	/// <param name="allLayers">the layers you want to save</param>
	void Serialize(std::vector<Layer>& allLayers, std::vector<BridgeConveyor>& allBridgeConveyors);
	/// <summary>
	/// loads from a file
	/// </summary>
	/// <param name="filePath">the place where the data is saved, relative to the exe</param>
	/// <param name="layerManager">used to set the returned vector as the layer vector used by the layerManager</param>
	void Deserialize(std::string& filePath, LayerManager& layerManager);

	void SerializeMouse(float& mouseSensitivity, float& camera3dfov);

	void DeserializeMouse(std::string& filePath, float& mouseSensitivity, float& camera3dfov);
};