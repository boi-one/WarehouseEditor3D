#include "LayerManager.h"

void Layer::DrawConveyors(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, glm::vec3& color)
{
	glm::vec3 localcolor = { 0, 1, 0 };
	shader.setVec3("mColor", localcolor);
	if (selectedConveyor && selectedConveyor->selectedPoint && selectedConveyor->edit) //draw newline
	{
		selectedConveyor->selectedPoint->position.z = depth;
		mouse.position.z = depth;
		cube.DrawLine(shader, localcolor, selectedConveyor->selectedPoint->position, mouse.position);
	}
	if (!orthoProjection)
		ConveyorManager::selectedConveyor = 0;
	localcolor = color;
	for (Conveyor& conveyor : allConveyors)
	{
		if (conveyor.edit) localcolor = { 0, 0, 1 };
		else if (conveyor.selected) localcolor = { 1, 1, 0 };
		else localcolor = color;
		conveyor.Draw(shader, cube, localcolor);
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
			c.selectedPoint = 0;
		}
		l.selected = false;
		l.selectedConveyor = 0;
	}
}

Conveyor* Layer::ReturnClosestConveyor(glm::vec3& origin)
{
	float smallestDistance = 99999;
	Point* closestPoint = 0;

	int closestConveyorIndex = -1;
	int closestPointIndex = -1;

	for (int c = 0; c < allConveyors.size(); c++)
	{
		Conveyor& conveyor = allConveyors[c];
		for (int p = 0; p < allConveyors[c].path.size(); p++)
		{
			Point& point = conveyor.path[p];
			float distance = glm::distance(point.position, origin);
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
	Point* closestPoint = 0;

	int closestConveyorIndex = -1;
	int closestPointIndex = -1;

	for (int c = 0; c < allConveyors.size(); c++)
	{
		Conveyor& conveyor = allConveyors[c];
		for (int p = 0; p < allConveyors[c].path.size(); p++)
		{
			Point& point = conveyor.path[p];

			float distance = glm::distance(point.position, origin);
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
	if (!closest) return false;
	if(closest != selectedConveyor->selectedPoint)
		closest->connections.push_back(*selectedConveyor->selectedPoint);
	for (Point& p : temp.path)
		selectedConveyor->path.push_back(p);
	
	UnselectConveyors();
	Tools::DeleteFromList(allConveyors, temp);
	return true;
}

void LayerManager::DrawLayers(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection)
{
	glm::vec3 color;
	int pointAmount = 0;
	for (Layer& layer : allLayers)
	{
		if (layer.hidden) continue;
		color = { 0.5f, 0.5f, 0.5f };
		if (layer.selected) color = { 1, 1, 1 };
		layer.DrawConveyors(shader, cube, mouse, orthoProjection, color);
	}
}