#include "LayerManager.h"

void Layer::DrawConveyors(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, glm::vec3& color, bool& gridSnap, bool& cast, glm::vec3& mousePos)
{

	glm::vec3 localcolor = { 0, 1, 0 };
	shader.setVec3("mColor", localcolor);
	if (selectedConveyor && selectedConveyor->selectedPoint && selectedConveyor->edit) //draw newline
	{
		selectedConveyor->selectedPoint->position.z = depth;
		mouse.position.z = depth;

		if (orthoProjection || cast)
		{
			glm::vec3 start = { selectedConveyor->selectedPoint->position.x, selectedConveyor->selectedPoint->position.y, depth };
			glm::vec3 end = { mousePos.x, mousePos.y, depth };
			cube.DrawLine(shader, localcolor, start, end, selectedConveyor->width, orthoProjection);
		}
	}
	localcolor = color;
	for (Conveyor& conveyor : allConveyors)
	{
		if (conveyor.edit) localcolor = { 0, 0, 1 };
		else if (conveyor.selected) localcolor = { 1, 1, 0 };
		else localcolor = color;
		conveyor.Draw(shader, cube, localcolor, orthoProjection);
	}
}

void LayerManager::UnselectEverything()
{
	for (Layer& l : allLayers)
	{
		for (Conveyor& c : l.allConveyors)
		{
			c.edit = false;
			c.selected = false;
			c.selectedPoint = nullptr;
			for (Point& p : c.path) p.selected = false;
		}
		l.selected = false;
		l.selectedConveyor = nullptr;
	}
}

Conveyor* LayerManager::FindClosestConveyorFromAll(glm::vec3 origin)
{
	float closestDistance = 999999;
	Conveyor* closestConveyor = nullptr;
	for (Layer& l : allLayers)
	{
		for (Conveyor& c : l.allConveyors)
		{
			for (Point& p : c.path)
			{
				float distance = glm::distance(origin, p.position);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestConveyor = &c;
				}
			}
		}
	}
	return closestConveyor;
}

Conveyor* Layer::ReturnClosestConveyor(glm::vec3& origin)
{
	float smallestDistance = 99999;
	Point* closestPoint = nullptr;

	int closestConveyorIndex = -1;
	int closestPointIndex = -1;

	for (int c = 0; c < allConveyors.size(); c++)
	{
		Conveyor& conveyor = allConveyors[c];
		for (int p = 0; p < allConveyors[c].path.size(); p++)
		{
			Point& point = conveyor.path[p];
			float distance = glm::distance(glm::vec2(point.position.x, point.position.y), glm::vec2(origin.x, origin.y));
			if (distance < smallestDistance)
			{
				smallestDistance = distance;
				closestPoint = &point;

				closestConveyorIndex = c;
				closestPointIndex = p;
			}
		}
	}

	//selected point gets selected here so when you select a new conveyor it instantly returns the closest point
	allConveyors[closestConveyorIndex].selectedPoint = &allConveyors[closestConveyorIndex].path[closestPointIndex];
	return &allConveyors[closestConveyorIndex];
}

Conveyor* Layer::ReturnClosestConveyor(glm::vec3& origin, Conveyor& selected)
{
	float smallestDistance = 99999;
	Point* closestPoint = nullptr;

	int closestConveyorIndex = -1;
	int closestPointIndex = -1;

	for (int c = 0; c < allConveyors.size(); c++)
	{
		Conveyor& conveyor = allConveyors[c];
		for (int p = 0; p < allConveyors[c].path.size(); p++)
		{
			Point& point = conveyor.path[p];

			float distance = glm::distance(glm::vec2(point.position.x, point.position.y), glm::vec2(origin.x, origin.y));
			if (distance < smallestDistance)
			{
				smallestDistance = distance;
				closestPoint = &point;

				if (&allConveyors[c] == &selected) continue;
				closestConveyorIndex = c;
				closestPointIndex = p;
			}
		}
	}

	//selected point gets selected here so when you select a new conveyor it instantly returns the closest point
	if (closestConveyorIndex > -1 || closestPointIndex > -1)
	{
		allConveyors[closestConveyorIndex].selectedPoint = &allConveyors[closestConveyorIndex].path[closestPointIndex];
		return &allConveyors[closestConveyorIndex];
	}
	return 0;
}

bool Layer::EditConveyor(glm::vec3& position, bool& orthoProjection)
{
	if (allConveyors.size() < 2 || !orthoProjection) return false;
	Conveyor& temp = *ReturnClosestConveyor(position, *selectedConveyor);
	Point* closest = temp.ClosestPoint(position, 100);
	if (!closest || !selectedConveyor->selectedPoint) return false;
	if (closest != selectedConveyor->selectedPoint)
	{
		closest->connections.push_back(*selectedConveyor->selectedPoint);
		temp.angle = selectedConveyor->angle;
	}
	for (Point& p : temp.path)
		selectedConveyor->path.push_back(p);

	UnselectConveyors();
	Tools::DeleteFromList(allConveyors, temp);
	return true;
}

void Layer::SetDepth(int layer)
{
	int multiply = 25;
	depth = layer * multiply;
	for (Conveyor& c : allConveyors)
	{
		for (Point& p : c.path)
		{
			p.depth = layer * multiply;
		}
	}
}

void LayerManager::DrawLayers(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, bool& gridSnap, bool& cast, glm::vec3& mousePos)
{
	for (BridgeConveyor& bg : allBridgeConveyors) bg.Draw();

	glm::vec3 color;
	int pointAmount = 0;
	for (Layer& layer : allLayers)
	{
		if (layer.hidden) continue;
		color = { 0.5f, 0.5f, 0.5f };
		if (layer.selected) color = { 1, 1, 1 };
		layer.DrawConveyors(shader, cube, mouse, orthoProjection, color, gridSnap, cast, mousePos);
	}
}

void LayerManager::UpdateBridgeConveyors(BridgeConveyor& bg)
{
	for (Layer& l : allLayers)
	{
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
			}
		}
	}
}

void to_json(json& j, const Layer& l)
{
	j = json
	{
		{"layerCount", l.layerCount},
		{"id", l.id},
		{"allConveyors", l.allConveyors},
		{"depth", l.depth}
	};
}

void from_json(const json& j, Layer& l)
{
	j.at("allConveyors").get_to(l.allConveyors);
	l.id = j.at("id").get<int>();
	l.layerCount = j.at("layerCount").get<int>();
	l.depth = j.at("depth").get<int>();
}

void to_json(json& j, const LayerManager& lm)
{

}


