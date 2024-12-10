#pragma once
#include "Mesh.h"

class Point
{
public:
	glm::vec3 position;
};

class Conveyor
{
public:
	bool selected;
	Mesh mesh;
	std::vector<Point> path;

	static Point* ClosestPoint(std::vector<Point> list, glm::vec3 origin, float range = 99999);

};

class ConveyorManager
{
	std::vector<Conveyor> allConveyors;
	void SelectConveyor(glm::vec3& mousePosition);
};



