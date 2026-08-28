#include "font.hpp"
#include <stb_truetype.h>



namespace jadeite
{
	
Font::Font(GLuint fontID, int width, int height, float fontSize, void* pData)
	: m_FontAtlasID{ fontID }
	, m_Width{ width }
	, m_Height{ height }
	, m_FontSize{ fontSize }
	, m_pFontData{ std::move(pData) }
{
	
}

Font::~Font()
{
	if ( m_FontAtlasID != 0 )
		glDeleteTextures( 1, &m_FontAtlasID );
	
	if ( m_pFontData )
	{
		typedef stbtt_bakedchar(stbtt_bakedchar)[96];
		delete[] (stbtt_bakedchar*)m_pFontData;
	}
}

FontGlyph Font::GetGlyph(char c, glm::vec2& pos)
{
	FontGlyph glyph{};
	
	if ( c >= 32 )
	{
		stbtt_aligned_quad quad;
		
		stbtt_GetBakedQuad(
			(stbtt_bakedchar*)(m_pFontData),
			m_Width, 
			m_Height, 
			c - 32, 
			&pos.x,
			&pos.y,
			&quad,
			1
		);
		
		glyph.min = Vertex{
			.position = glm::vec2{ quad.x0, quad.y0 },
			.uvs = UV{ .u = quad.s0, .v = quad.t0 }
		};
		
		glyph.max = Vertex{
			.position = glm::vec2{ quad.x1, quad.y1 },
			.uvs = UV{ .u = quad.s1, .v = quad.t1 }
		};
	}
	
	return glyph;
}

void Font::GetNextCharPos(char c, glm::vec2& pos)
{
	if ( c >= 32 )
	{
		stbtt_aligned_quad quad;
		
		stbtt_GetBakedQuad(
			(stbtt_bakedchar*)(m_pFontData),
			m_Width, 
			m_Height, 
			c - 32, 
			&pos.x,
			&pos.y,
			&quad,
			1
		);
	}
}

} // jadeite
