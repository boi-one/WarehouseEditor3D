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
	glm::vec3 color = { 1, 1, 1 };
	if (!orthoProjection)
		ConveyorManager::selectedConveyor = 0;
	if (ConveyorManager::selectedConveyor && orthoProjection)
	{
		color = { 1, 0, 0 };
		shader.setVec3("mColor", color);
		Conveyor::DrawNewLine(ConveyorManager::selectedConveyor->selectedPoint->position, mousePosition, shader);
		color = { 1, 1, 1 };
	}

	for (Conveyor& c : ConveyorManager::allConveyors) //draw lines
	{
		shader.setVec3("mColor", color);
		for (int i = 0; i < c.path.size() - 1; i++)
		{
			c.DrawLine(c.path.at(i).position, c.path.at(i + 1).position, shader);
		}
	}
	for (Conveyor& c : ConveyorManager::allConveyors)
	{
		for (Point& p : c.path) //draw points
		{
			glm::vec3 color = { 1, 1, 1 };

			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, p.position);
			model = glm::scale(model, { 11, 11, 11 });
			shader.setMat4("model", model);

			if (ConveyorManager::selectedConveyor && ConveyorManager::selectedConveyor->selectedPoint && &p == ConveyorManager::selectedConveyor->selectedPoint)
			{
				color = { 1, 0, 0 };
			}

			shader.setVec3("mColor", color);
			cube.Draw(shader);
		}
	}
}

Point* Conveyor::ClosestPoint(std::vector<Point> list, glm::vec3 origin, float range)
{
	float closestDistance = range;
	Point* point = 0;

	for (Point& p : list)
	{
		float distance = glm::distance(p.position, origin);
		if (distance < closestDistance)
		{
			closestDistance = distance;
			point = &p;
		}
	}
	return point;
}

void Conveyor::DrawLine(glm::vec3& start, glm::vec3& end, Shader& shader)
{

	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 pivot = (start + end);
	model = glm::translate(model, pivot / 2.f);

	const double pi = 3.14159265358979323846;
	float meshAngle = atan2(start.y - end.y, start.x - end.x);
	model = glm::rotate(model, meshAngle, { 0, 0, 1 });

	float length = glm::distance(start, end);
	mesh->scale.x = length / 2.f;
	model = glm::scale(model, mesh->scale);

	shader.setMat4("model", model);
	mesh->Draw(shader);
}

void Conveyor::Place(glm::vec3 position)
{
	if (!ConveyorManager::selectedConveyor)
	{
		Conveyor c;
		c.path.push_back(glm::vec3(position.x, position.y, -1));
		c.selectedPoint = &c.path.at(c.path.size() - 1);
		ConveyorManager::allConveyors.push_back(c);
		ConveyorManager::selectedConveyor = &ConveyorManager::allConveyors.at(ConveyorManager::allConveyors.size() - 1);
		ConveyorManager::selectedConveyor->selectedPoint = &ConveyorManager::selectedConveyor->path.at(ConveyorManager::selectedConveyor->path.size() - 1);
	}
	else
	{
		ConveyorManager::selectedConveyor->path.push_back(glm::vec3(position.x, position.y, -1));
		ConveyorManager::selectedConveyor->selectedPoint = &ConveyorManager::selectedConveyor->path.at(ConveyorManager::selectedConveyor->path.size() - 1);
	}
}

void Conveyor::DrawNewLine(glm::vec3& start, glm::vec3& end, Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 pivot = (start + end);
	model = glm::translate(model, pivot / 2.f);

	double pi = 3.14159265358979323846;
	float meshAngle = atan2(start.y - end.y, start.x - end.x);
	model = glm::rotate(model, meshAngle, { 0, 0, 1 });

	float length = glm::distance(start, end);
	mesh->scale.x = length / 2.f;
	model = glm::scale(model, mesh->scale);

	shader.setMat4("model", model);
	mesh->Draw(shader);
}