#include "texture.hpp"

namespace jadeite
{
Texture::Texture()
	: Texture(0, 0, 0, "")
{
}

Texture::Texture(GLuint id, int width, int height, const std::string& sPath)
	: m_TextureID{ id }
	, m_Width{ width }
	, m_Height{ height }
	, m_sPath{ sPath }
{
}

void Texture::Enable()
{
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Disable()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::CreateLuaBind(sol::state& lua)
{
	lua.new_usertype<Texture>(
		"Texture",
		sol::no_constructor,
		"width", &Texture::m_Width,
		"height", &Texture::m_Height
	);
}

} // jadeite
