#pragma once
#include "vertex.hpp"
#include <GLES3/gl3.h>

namespace jadeite
{

struct FontGlyph
{
	Vertex min;
	Vertex max;
};

class Font
{
public:
	Font(GLuint fontID, int width, int height, float fontSize, void* pData);
	~Font();
	
	FontGlyph GetGlyph(char c, glm::vec2& pos);
	void GetNextCharPos(char c, glm::vec2& pos);
	
	inline GLuint GetFontAtlasID() const { return m_FontAtlasID; }
	inline float GetFontSize() const { return m_FontSize; }
	
private:
	GLuint m_FontAtlasID;
	int m_Width;
	int m_Height;
	float m_FontSize;
	void* m_pFontData;
};

} // jadeite
