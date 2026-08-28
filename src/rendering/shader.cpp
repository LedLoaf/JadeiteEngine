#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

namespace jadeite
{
Shader::Shader(GLuint program)
	: m_ShaderProgram{ program }
{
}

Shader::~Shader()
{
	glDeleteProgram(m_ShaderProgram);
}

void Shader::Enable()
{
	glUseProgram(m_ShaderProgram);
}

void Shader::Disable()
{
	glUseProgram(0);
}

void Shader::SetUniformMat4(const std::string& sName, glm::mat4& mat)
{
	glUniformMatrix4fv(GetUniformLocation(sName), 1, GL_FALSE, &mat[0][0]);
}

glm::mat4 Shader::GetUniformMat4(const std::string& sName)
{
	GLfloat values[16];
	glGetUniformfv(m_ShaderProgram, GetUniformLocation(sName), values);
	return glm::make_mat4(values);
}

GLuint Shader::GetUniformLocation(const std::string& sName)
{
	auto uniformItr = m_mapUniformLocations.find(sName);
	if (uniformItr != m_mapUniformLocations.end())
		return uniformItr->second;
	
	GLuint location = glGetUniformLocation(m_ShaderProgram, sName.c_str());
	if (location == 0xFFFFFFFF)
	{
		std::cerr << "Uniform [" << sName << "] not found in shader.\n";
		return -1;
	}
	
	m_mapUniformLocations.emplace(sName, location);
	
	return location;
}

} // jadeite
