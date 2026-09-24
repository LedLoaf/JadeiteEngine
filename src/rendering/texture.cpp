#include "texture.hpp"

namespace jadeite
{
	
/* Default constructor */
Texture::Texture()
	: Texture(0, 0, 0, "")
{
}

/* Creates a texture id with width, height, and the path to the file */
Texture::Texture(GLuint id, int width, int height, const std::string& sPath)
	: m_TextureID{ id }
	, m_Width{ width }
	, m_Height{ height }
	, m_sPath{ sPath }
{
}

/* Enable the texture */
void Texture::Enable()
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

/* Disables the texture */
void Texture::Disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

/* The bindings for lua to access */
void Texture::CreateLuaBind(sol::state& lua)
{
	lua.new_usertype<Texture>(
		"Texture",
		sol::no_constructor,
		"width", &Texture::m_Width,
		"height", &Texture::m_Height
	);
}

} // jadeite::Texture
