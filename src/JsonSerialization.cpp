#include "JsonSerialization.h"

void JsonSerialization::Serialize(std::vector<Layer>& allLayers)
{
	json jsonFile;

	jsonFile =
	{
		{"allLayers", allLayers}
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
		layerManager.selectedLayer->selectedConveyor = 0;
		std::cout << "deserialization success" << std::endl;
	}
	else std::cout << "deserialization failed, can't open file" << std::endl;
}
