#include "Conveyor.h"

void ConveyorManager::SelectConveyor(glm::vec3& mousePosition)
{
	for (Conveyor& c : allConveyors)
	{

		Conveyor::ClosestPoint(c.path, mousePosition);
	}

}

void ConveyorManager::RenderConveyors(Shader& shader, Mesh& cube, bool& orthoProjection, glm::vec3 mousePosition)
{

}

void Conveyor::Draw(Shader& shader, Mesh& cube, glm::vec3& color)
{
	for (Point& p : path)
	{
		p.Draw(color, shader);
	}
}

Point* Conveyor::ClosestPoint(std::vector<Point> list, glm::vec3 origin, float range)
{
	float closestDistance = range;
	Point* point = 0;

	for (Point& p : list)
	{
		float distance = glm::distance(p.position, origin);
		for(Point& c : p.connections)
			distance = glm::distance(c.position, origin);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			point = &p;
		}
	}
	return point;
}

void Conveyor::Place(glm::vec3 position)
{
	path.push_back({ position, mesh });
	selectedPoint = &path[path.size() - 1];
	if (path.size() > 1) selectedPoint->connections.push_back(path[path.size() - 2]);
}

void Point::Draw(glm::vec3& color, Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, { position.x, position.y, depth });
	model = glm::scale(model, { 11, 11, 11 });
	shader.setMat4("model", model);

	if (ConveyorManager::selectedConveyor && ConveyorManager::selectedConveyor->selectedPoint && this == ConveyorManager::selectedConveyor->selectedPoint)
	{
		color = { 1, 0, 0 };
	}

	shader.setVec3("mColor", color);
	mesh->Draw(shader);

	for (Point& connection : connections)
	{
		mesh->DrawLine(shader, color, position, connection.position); //lines between points
	}
}
