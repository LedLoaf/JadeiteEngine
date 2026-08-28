#pragma once
#include <GLES3/gl3.h>
#include <string>
#include <sol/sol.hpp>

namespace jadeite
{
	
class Texture
{
public:
	Texture();
	Texture(GLuint id, int width, int height, const std::string& sPath);
	
	void Enable();
	void Disable();
	
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline GLuint GetID() const { return m_TextureID; }
	
	static void CreateLuaBind(sol::state& lua);
private:
	GLuint m_TextureID;
	int m_Width;
	int m_Height;
	std::string m_sPath;
};

} // jadeite