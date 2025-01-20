#pragma once
#include <GL/glew.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// Made so the shaders can be accessed more easily
/// </summary>
class Shader
{
public:
	unsigned int ID;

	Shader(const char* vertexPath, const char* fragmentPath);

	/// <summary>
	/// makes the current shader the active program on the graphics card
	/// </summary>
	void use();
	/// <summary>
	/// sends a bool to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setBool(const std::string& name, bool value) const;
	/// <summary>
	/// sends an int to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setInt(const std::string& name, int value) const;
	/// <summary>
	/// sends a float to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setFloat(const std::string& name, float value) const;
	/// <summary>
	/// sends a glm::vec3 to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setVec3(const std::string& name, const glm::vec3& value) const;
	/// <summary>
	/// sends a glm::mat2 to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setMat2(const std::string& name, const glm::mat2& value) const;
	/// <summary>
	/// sends a glm::mat3 to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setMat3(const std::string& name, const glm::mat3& value) const;
	/// <summary>
	/// sends a glm::mat4 to a uniform on the graphics card
	/// </summary>
	/// <param name="name">the name of the uniform</param>
	/// <param name="value">the variable you want to send</param>
	void setMat4(const std::string& name, const glm::mat4& value) const;
};

