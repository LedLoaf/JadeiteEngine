#pragma once
#include "vertex.hpp"
#include <GLES3/gl3.h>

namespace jadeite
{
	/* Font glyph of just the min and max vertex */
	struct FontGlyph
	{
		Vertex min;
		Vertex max;
	};

	/* Used for the type of text style */
	class Font
	{
	public:
		/* Constructor that takes the fontID, width, height, size to create the font, size to render the font, and the font data */
		Font(GLuint fontID, int width, int height, float bakedSize, float renderSize, void* pData);
		~Font();
		
		FontGlyph GetGlyph(char c, glm::vec2& pos);
		void GetNextCharPos(char c, glm::vec2& pos);
		
		inline GLuint GetFontAtlasID() const { return m_FontAtlasID; }
		/* The font size it was created at. This will typically be 64 and downscaled to the appropiate size */
		inline float GetBakedSize() const { return m_BakedSize; }
		/* The font size that this is to be rendered at */
		inline float GetRenderSize() const { return m_RenderSize; }
	private:
		GLuint m_FontAtlasID;
		int m_Width;
		int m_Height;
		float m_BakedSize;				// the size in which the font was created
		float m_RenderSize;	 			// the font size we want when we render the font.
		void* m_pFontData;
	};
} // jadeite
