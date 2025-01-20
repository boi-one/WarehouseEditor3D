#pragma once
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <string>

#include "LayerManager.h"

class JsonSerialization
{
public:
	Mesh* cube = 0;
public:
	void LateConstruct(Mesh* cube)
	{
		this->cube = cube;
	}
	void Serialize(std::vector<Layer>& allLayers);
	void Deserialize(std::string& filePath, LayerManager& layerManager);
};