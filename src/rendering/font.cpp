#include "font.hpp"
#include <stb_truetype.h>

namespace jadeite
{
	/* Constructor that takes the font id, width, height, font size, and a pointer to the data */
	Font::Font(GLuint fontID, int width, int height, float bakedSize, float renderSize, void* pData)
		: m_FontAtlasID{ fontID }
		, m_Width{ width }
		, m_Height{ height }
		, m_BakedSize{ bakedSize }
		, m_RenderSize{ renderSize }
		, m_pFontData{ std::move(pData) }
	{
		
	}

	/* Destructor that clears the font data */
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

	/* Retrieve the font glyph */
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
			
		// This is needed to get the text to appropiately appear in the right place.
		// e.g. Scale = (12 size font / 64 size font) = 0.1875 scale
		float scale = m_RenderSize / m_BakedSize; 

		// This scale factor must be applied to the glyph vertex position to properly appear on screen.
        glyph.min = Vertex{
            .position 	= glm::vec2{ quad.x0 * scale, quad.y0 * scale },
            .uvs 		= UV{ .u = quad.s0, .v = quad.t0 }
        };
        glyph.max = Vertex{
            .position 	= glm::vec2{ quad.x1 * scale, quad.y1 * scale },
            .uvs 		= UV{ .u = quad.s1, .v = quad.t1 }
        };
		}
		
		return glyph;
	}

	/* Helper function to retrieve the next character */
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
} // jadeite::Font