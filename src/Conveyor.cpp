#include "Conveyor.h"


Point* Conveyor::ClosestPoint(glm::vec3& origin, float range)
{
	Point* closestPoint = 0;

	for (Point& p : path)
	{
		float distance = glm::distance(glm::vec2(p.position.x, p.position.y), glm::vec2(origin.x, origin.y));
		if (distance < range)
		{
			range = distance;
			closestPoint = &p;
		}
	}

	return closestPoint;
}

void Conveyor::Draw(Shader& shader, Mesh& cube, glm::vec3& color)
{
	for (Point& p : path)
	{
		p.Draw(color, shader);
	}
}

void Conveyor::NewPoint(glm::vec3 position)
{
	if (!selectedPoint)
	{
		selectedPoint = &path.emplace_back(Point(position, mesh));
		return;
	}

	Point& temp = selectedPoint->connections.emplace_back(Point(position, mesh));
	path.emplace_back(temp);
	selectedPoint = &path[path.size() - 1];
}

void Point::Draw(glm::vec3& color, Shader& shader)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, { position.x, position.y, depth });
	model = glm::scale(model, { 11, 11, 11 });
	shader.setMat4("model", model);
	shader.setVec3("mColor", color);
	mesh->Draw(shader);

	for (Point& connection : connections)
	{
		glm::vec3 start = { position.x, position.y, depth };
		glm::vec3 end = { connection.position.x, connection.position.y, depth };
		mesh->DrawLine(shader, color, start, end);
	}
}

void to_json(json& j, const Point& p)
{
	j = json
	{
		{"x", p.position.x}, {"y", p.position.y},
		{"connections", p.connections},
		{"depth", p.depth}
	};
}

void from_json(const json& j, Point& p)
{
	p.position.x = j.at("x").get<float>();
	p.position.y = j.at("y").get<float>();
	p.depth = j.at("depth").get<int>();

	//deserialize the connections vector
	if (j.contains("connections"))
	{
		j.at("connections").get_to(p.connections);
	}
}

void to_json(json& j, const Conveyor& c)
{
	j = json
	{
		{"alltimeConveyorCount", c.alltimeConveyorCount},
		{"path", c.path},
		{"id", c.id},
	};
}

void from_json(const json& j, Conveyor& c)
{
	c.alltimeConveyorCount = j.at("alltimeConveyorCount").get<int>();
	c.id = j.at("id").get<int>();
	j.at("path").get_to<std::vector<Point>>(c.path);
}
