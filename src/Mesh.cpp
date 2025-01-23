#include "Mesh.h"

void Mesh::CreateCube()
{
	GLfloat vertices[] =
	{
		//voor (maar omdat nu alles is gedraaid boven en onder)
		 1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//0
		 1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//1
		-1.0f, -1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//2
		-1.0f,  1.0f,  1.0f,    1.0f, 1.0f, 1.0f,//3
		//achter	   (onder) 
		 1.0f,  1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//4
		 1.0f, -1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//5
		-1.0f, -1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//6
		-1.0f,  1.0f, -1.0f,    0.5f, 0.5f, 0.5f,//7
	};

	GLuint indices[] =
	{
		0, 1, 2,
		0, 2, 3,

		3, 2, 6,
		3, 6, 7,

		7, 6, 5,
		7, 5, 4,

		4, 5, 1,
		4, 1, 0,

		5, 1, 6,
		1, 2, 6,

		4, 0, 7,
		0, 3, 7,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);
	//color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	verticesCount = sizeof(indices) / sizeof(GLuint);
}

void Mesh::Draw(Shader& shader)
{
	shader.use();
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glDrawElements(GL_TRIANGLES, verticesCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::RenderAxis(Shader& shader, bool& showAxes, float pixelSize, glm::vec3 position)
{
	int axes = 0;
	if (showAxes) axes = 3;
	if (axes > 0)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, { 0, 0, 0 });
		model = glm::scale(model, { 3 * pixelSize, 3 * pixelSize, 3 * pixelSize });
		shader.setMat4("model", model);
		shader.setVec3("mColor", { 1, 1, 1 });
		Draw(shader);
		shader.setVec3("mColor", { 1, 0, 0 });
	}
	for (int i = 0; i < axes; i++)
	{
		glm::mat4 model = glm::mat4(1.0f);
		if (i == 0)
		{
			shader.setVec3("mColor", { 1, 0, 0 });
			model = glm::translate(model, { position.x, 0, 0 });
			model = glm::scale(model, { 9000, pixelSize, pixelSize });
			shader.setMat4("model", model);
		}
		if (i == 1)
		{
			shader.setVec3("mColor", { 0, 1, 0 });
			model = glm::translate(model, { 0, position.y, 0 });
			model = glm::scale(model, { pixelSize, 9000, pixelSize });
			shader.setMat4("model", model);
		}
		if (i == 2)
		{
			shader.setVec3("mColor", { 0, 0, 1 });
			model = glm::translate(model, { 0, 0, position.z });
			model = glm::scale(model, { pixelSize, pixelSize, 9000 });
			shader.setMat4("model", model);
		}

		Draw(shader);
	}
	shader.setVec3("mColor", { 1, 1, 1 });
}

void Mesh::DrawLine(Shader& shader, glm::vec3& color, glm::vec3& start, glm::vec3& end, bool ortho)
{
	shader.setVec3("mColor", color);

	glm::mat4 model = glm::mat4(1.0f);
	glm::vec3 pivot = (start + end);
	model = glm::translate(model, pivot / 2.f);

	const double pi = 3.14159265358979323846;
	float angle = 0;
	if (ortho) //2d rotation
	{
		angle = atan2(start.y - end.y, start.x - end.x);
		model = glm::rotate(model, angle, { 0, 0, 1 });
	}
	else //3d rotation
	{
		glm::vec3 direction = glm::normalize(end - start);
		const glm::vec3 defaultAxis = { 1.0f, 0.0f, 0.0f };
		glm::vec3 rotationAxis = glm::cross(defaultAxis, direction);
		angle = acos(glm::dot(defaultAxis, direction));
		model = glm::rotate(model, angle, glm::normalize(rotationAxis));
	}

	float length = glm::distance(start, end);
	scale.x = length / 2.f;
	model = glm::scale(model, scale);

	shader.setMat4("model", model);
	Draw(shader);

	shader.setVec3("mColor", { 1, 1, 1 });
}
