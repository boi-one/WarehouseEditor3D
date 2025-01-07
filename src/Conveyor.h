#pragma once
#include "Mesh.h"
#include "Shader.h"

class Point
{
public:
	glm::vec3 position;
	std::vector<Point> connections;

	Point(glm::vec3 position)
	{
		this->position = position;
	}

};

class Conveyor
{
public:
	bool selected;
	Mesh* mesh = 0;
	std::vector<Point> path;
	Point* selectedPoint = 0;

	static Point* ClosestPoint(std::vector<Point> list, glm::vec3 origin, float range = 99999);
	void DrawLine(glm::vec3& start, glm::vec3& end, Shader& shader);
};

class ConveyorManager
{
public:
	static inline Conveyor* selectedConveyor = 0;
	static inline std::vector<Conveyor> allConveyors;
	void SelectConveyor(glm::vec3& mousePosition);
	static void DrawNewLine(glm::vec3& start, glm::vec3& end, Mesh& mesh, Shader& shader);
};



