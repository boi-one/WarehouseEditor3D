#include "JsonSerialization.h"

void JsonSerialization::Serialize(std::vector<Layer>& allLayers, std::vector<BridgeConveyor>& allBridgeConveyors)
{
	json jsonFile;

	jsonFile =
	{
		{"allLayers", allLayers},
		{"allBridgeConveyors", allBridgeConveyors}
	};

	std::ofstream file("save.json");

	if (file.is_open())
	{
		file << jsonFile.dump(4);
		file.close();
		std::cout << "serialization success" << std::endl;
	}
	else std::cout << "failed serialization" << std::endl;
}

void JsonSerialization::Deserialize(std::string& filePath, LayerManager& layerManager)
{
	std::ifstream file(filePath);

	if (file.is_open())
	{
		json data;
		file >> data;

		std::vector<Layer> allLayers = data.at("allLayers").get<std::vector<Layer>>();
		for (Layer& l : allLayers)
		{
			for (Conveyor& c : l.allConveyors)
			{
				c.mesh = this->cube;
				for (Point& p : c.path)
					p.mesh = this->cube;

			}
		}
		layerManager.allLayers = allLayers;
		layerManager.UnselectEverything();
		layerManager.selectedLayer = &layerManager.allLayers[0];
		layerManager.selectedLayer->selected = true;
		layerManager.selectedLayer->selectedConveyor = nullptr;

		std::vector<BridgeConveyor> allBridgeConveyors = data.at("allBridgeConveyors").get<std::vector<BridgeConveyor>>();
		for (BridgeConveyor& bg : allBridgeConveyors)
		{
			for (Layer& l : layerManager.allLayers)
			{
				bg.mesh = layerManager.cube;
				bg.shader = layerManager.shader;
				for (Conveyor& c : l.allConveyors)
				{
					for (Point& p : c.path)
					{
						if (bg.startPointID == p.id)
						{
							bg.startPoint = &p;
							bg.startPoint->position.z = l.depth;
						}
						if (bg.endPointID == p.id)
						{
							bg.endPoint = &p;
							bg.endPoint->position.z = l.depth;
						}
						if (bg.startPoint && bg.endPoint) break;
					}
					if (bg.startPoint && bg.endPoint) break;
				}
				if (bg.startPoint && bg.endPoint) break;
			}
		}
		layerManager.allBridgeConveyors = allBridgeConveyors;
		std::cout << "deserialization success" << std::endl;
	}
	else std::cout << "deserialization failed, can't open file" << std::endl;
}

void JsonSerialization::SerializeMouse(float& mouseSensitivity, float& camera3dfov)
{
	json jsonFile;

	jsonFile = {
		{"mouseSensitivity", mouseSensitivity},
		{"camera3dfov", camera3dfov}
	};

	std::ofstream file("mouse.json");

	if (file.is_open())
	{
		file << jsonFile.dump(4);
		file.close();
		std::cout << "mouse serialization success" << std::endl;
	}
	else std::cout << "mouse failed serialization" << std::endl;
}

void JsonSerialization::DeserializeMouse(std::string& filePath, float& mouseSensitivity, float& camera3dfov)
{
	std::ifstream file(filePath);

	if (file.is_open())
	{
		json data;
		file >> data;
		mouseSensitivity = data.at("mouseSensitivity").get<float>();
		camera3dfov = data.at("camera3dfov").get<float>();
		std::cout << "deserialization success" << std::endl;
	}
	else std::cout << "deserialization failed, can't open file" << std::endl;
}
