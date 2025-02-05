#include "Conveyor.h"


Point* Conveyor::ClosestPoint(glm::vec3& origin, float range)
{
	Point* closestPoint = nullptr;

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

void Conveyor::Draw(Shader& shader, Mesh& cube, glm::vec3& color, bool& ortho)
{
	for (Point& p : path)
	{
		p.Draw(color, shader, angle, ortho, width, path[0].position);
	}
}

void Conveyor::NewPoint(glm::vec3 position)
{
	if (!selectedPoint)
	{
		selectedPoint = &path.emplace_back(Point(position, mesh));
		return;
	}
	Point newPoint(position, mesh);
	selectedPoint->connections.push_back(newPoint);
	path.push_back(newPoint);
	selectedPoint = &path[path.size() - 1];

	for (Point& p : path) p.selected = false;
	selectedPoint->selected = true;
}

glm::vec3 Conveyor::GetAveragePosition()
{
	int amount = 0;
	float totalX = 0;
	float totalY = 0;
	for (Point& p : this->path)
	{
		amount++;
		totalX += p.position.x;
		totalY += p.position.y;
	}

	return glm::vec3(totalX / amount, totalY / amount, path[0].position.z);
}

void Conveyor::Rotate(int direction)
{
	if (!this) return;

	glm::vec3 pivot = GetAveragePosition();

	for (Point& p : path)
	{
		glm::vec4 tempPos = { p.position - pivot, 1 };
		glm::mat4 rotation = glm::mat4(1.0f);
		rotation = glm::rotate(rotation, glm::radians((float)direction), { 0, 0, 1 });
		glm::vec4 result = tempPos * rotation;
		for (Point& pc : p.connections)
		{
			glm::vec4 tempPospc = { pc.position - pivot, 1 };
			glm::vec4 resultpc = tempPospc * rotation;
			pc.position = glm::vec3(resultpc.x, resultpc.y, pc.position.z) + pivot;
		}
		p.position = glm::vec3(result.x, result.y, p.position.z) + pivot;
	}
}

void Point::Draw(glm::vec3& color, Shader& shader, float angle, bool& ortho, float width, glm::vec3 averagePos)
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, { position.x, position.y, depth });
	model = glm::scale(model, { width, width, 11 });
	shader.setMat4("model", model);
	shader.setVec3("mColor", color);
	mesh->Draw(shader);

	for (Point& connection : connections)
	{
		glm::vec3 start = { position.x, position.y, depth };
		glm::vec3 end = { connection.position.x, connection.position.y, depth };
		mesh->DrawLine(shader, color, start, end, width, ortho);
	}
}

void to_json(json& j, const Attribute& a)
{
	j = json
	{
		{"name", a.name},
		{"isNumber", a.isNumber}
	};
	if (a.isNumber)	j["value"] = std::stod(a.value);
	else j["value"] = a.value;

}

void from_json(const json& j, Attribute& a)
{
	a.name = j.at("name").get<std::string>();
	a.isNumber = j.at("isNumber").get<bool>();
	if(a.isNumber) a.value = j.at("value").get<double>();
	else a.value = j.at("value").get<std::string>();
}

void to_json(json& j, const Point& p)
{
	j = json
	{
		{"id", p.id},
		{"x", p.position.x}, {"y", p.position.y},
		{"connections", p.connections},
		{"depth", p.depth}
	};
}

void from_json(const json& j, Point& p)
{
	p.id = j.at("id").get<int>();
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
		{"attributes", c.attributes},
		{"path", c.path},
		{"id", c.id},
	};
}

void from_json(const json& j, Conveyor& c)
{
	c.alltimeConveyorCount = j.at("alltimeConveyorCount").get<int>();
	c.id = j.at("id").get<int>();
	j.at("path").get_to<std::vector<Point>>(c.path);
	j.at("attributes").get_to<std::vector<Attribute>>(c.attributes);
}
