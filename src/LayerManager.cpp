#include "LayerManager.h"

void Layer::DrawConveyors(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection, glm::vec3& color)
{
	glm::vec3 localcolor = { 0, 1, 0 };
	shader.setVec3("mColor", localcolor);
	if (selectedConveyor && selectedConveyor->selectedPoint) //draw newline
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

void LayerManager::DrawLayers(Shader& shader, Mesh& cube, Mouse& mouse, bool& orthoProjection)
{
	glm::vec3 color;
	for (Layer& layer : allLayers)
	{
		if (layer.hidden) continue;
		color = { 0.5f, 0.5f, 0.5f };
		if (layer.selected) color = { 1, 1, 1 };
		layer.DrawConveyors(shader, cube, mouse, orthoProjection, color);
	}
}