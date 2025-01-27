#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include "Shader.h"

enum Shape
{
	CUBE,
};
/// <summary>
/// A shape in the world you can render
/// </summary>
class Mesh
{
public:
	glm::vec3 scale;
private:
	GLuint VAO = 0;
	GLuint VBO = 0;
	GLuint EBO = 0;
	glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 right;
	int verticesCount = 0;
public:
	/// <summary>
	/// Create a mesh, when using the default constructor the shape is always a cube
	/// </summary>
	Mesh(glm::vec3 scale = { 10, 10, 10 })
	{
		this->scale = scale;
		CreateCube();
	}
	/// <summary>
	/// Create a mesh
	/// </summary>
	/// <param name="shape"> is the shape of the mesh</param>
	/// <param name="scale"> is the scale of the mesh</param>
	Mesh(Shape shape, glm::vec3 scale = { 10, 10, 10 })
	{
		this->scale = scale;

		switch (shape)
		{
		case Shape::CUBE:
			CreateCube();
			break;
		}
	}
	/// <summary>
	/// sets the shape of the mesh to a cube
	/// </summary>
	void CreateCube();
	/// <summary>
	/// renders the mesh
	/// </summary>
	/// <param name="shader">used for the transforms</param>
	void Draw(Shader& shader);
	/// <summary>
	/// renders the xyz axes so you can orient yourself in the 3D world
	/// </summary>
	/// <param name="shader">used for the transforms</param>
	/// <param name="showAxes">if the axes should be shown or not</param>
	void RenderAxis(Shader& shader, bool& showAxes, float pixelSize, glm::vec3 position);
	/// <summary>
	/// draws a line between two points
	/// </summary>
	/// <param name="shader">used for the transform</param>
	/// <param name="color">the color of the line</param>
	/// <param name="start">the start position of the line </param>
	/// <param name="end">the end position of the line</param>
	/// <param name="ortho">if the line should be rotated in 2D or 3D</param>
	void DrawLine(Shader& shader, glm::vec3& color, glm::vec3& start, glm::vec3& end);
};