#pragma once
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

namespace jadeite
{
class Shader
{
public:
	Shader(GLuint program);
	~Shader();
	
	void Enable();
	void Disable();
	
	void SetUniformMat4(const std::string& sName, glm::mat4& mat);
	glm::mat4 GetUniformMat4(const std::string& sName);
	
	inline GLuint GetID() const { return m_ShaderProgram; }
	
private:
	GLuint GetUniformLocation(const std::string& sName);
	
private:
	GLuint m_ShaderProgram;
	std::unordered_map<std::string, GLuint> m_mapUniformLocations;
};
} // jadeite
