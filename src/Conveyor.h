#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include "Mesh.h"
#include "Shader.h"
#include "Mouse.h"

class Point
{
public:
	static inline int alltimePointCount = 0;
	glm::vec3 position;
	std::vector<Point> connections;
	bool selected = false;
	Mesh* mesh = 0;
	/// <summary>
	/// depth is how high the point is in 3D, the layer class has also a depth, because of the near clip plane of the orthographic camera you can max have 40 layers but if you decrease it to something like -2000 it will display more layers
	/// </summary>
	float depth = 0;
	int id;

public:
	Point() = default;
	Point(glm::vec3 position, Mesh* mesh)
	{
		this->position = position;
		this->mesh = mesh;
		id = alltimePointCount++;
	}

	/// <summary>
	/// Draws the point and all of the connections of this specific point
	/// </summary>
	/// <param name="color"> is the color of this point</param>
	/// <param name="camera"> is used to convert to world position</param>
	/// <param name="thickness"> is how thick the lines between the points are </param>
	void Draw(glm::vec3& color, Shader& shader);
	int ID() const { return id; };

	friend void to_json(json& j, const Point& p);
};

void from_json(const json& j, Point& p);

/// <summary>
/// visualizes a conveyor
/// </summary>
class Conveyor
{
public:
	static inline int alltimeConveyorCount = 0;
	/// <summary>
	/// these points make up the conveyor
	/// </summary>
	std::vector<Point> path;
	bool selected = false;
	bool edit = false;
	Point* selectedPoint = 0;
	static inline Mesh* mesh = 0;
	int id;

public:
	Conveyor()
	{
		id = alltimeConveyorCount++;
	}
	/// <summary>
	/// returns the closest point of the conveyor, a conveyor is a list of points with lines rendered between them
	/// </summary>
	/// <param name="origin">position to check which point is closest to it</param>
	/// <param name="range">the range around the origin so you won't have to check every point</param>
	/// <returns>a pointer to the closest point in the conveyor</returns>
	Point* ClosestPoint(glm::vec3& origin, float range);
	/// <summary>
	/// draws the conveyor onto the screen
	/// </summary>
	/// <param name="shader">used for transforms</param>
	/// <param name="cube">used to render the conveyor</param>
	/// <param name="color">used for the color of the conveyor</param>
	void Draw(Shader& shader, Mesh& cube, glm::vec3& color);
	/// <summary>
	/// adds a point to the conveyor
	/// </summary>
	/// <param name="position">place where you want the point to be</param>
	void NewPoint(glm::vec3 position);
	/// <summary>
	/// 
	/// </summary>
	/// <returns> the ID of the conveyor </returns>
	int ID() const { return id; };

	friend void to_json(json& j, const Conveyor& c);
};

void from_json(const json& j, Conveyor& c);



