#include "Conveyor.h"

void ConveyorManager::SelectConveyor(glm::vec3& mousePosition)
{
    for (Conveyor& c : allConveyors)
    {

        Conveyor::ClosestPoint(c.path, mousePosition);
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
	mesh.scale.x = length / 2.f;
	model = glm::scale(model, mesh.scale);

	shader.setMat4("model", model);
	mesh.Draw(shader);
}

void Conveyor::Place(glm::vec3 position)
{
	if (!ConveyorManager::selectedConveyor)
	{
		Conveyor c;
		c.path.push_back(glm::vec3(position.x, position.y, -1));
		
		std::cout << c.path.size() << std::endl;
		
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

void ConveyorManager::DrawNewLine(glm::vec3& start, glm::vec3& end, Mesh& mesh, Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 pivot = (start + end);
	model = glm::translate(model, pivot / 2.f);

	double pi = 3.14159265358979323846;
	//float meshAngle = -atan2(start.y, start.x) + atan2(end.y, end.x) + 1 * pi / 2; //kan ook in 1 keer
	float meshAngle = atan2(start.y - end.y, start.x - end.x);
	model = glm::rotate(model, meshAngle, { 0, 0, 1 });

	float length = glm::distance(start, end);
	mesh.scale.x = length / 2.f;
	model = glm::scale(model, mesh.scale);

	shader.setMat4("model", model);
	mesh.Draw(shader);
}