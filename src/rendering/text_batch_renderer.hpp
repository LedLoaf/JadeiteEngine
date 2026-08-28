#pragma once 
#include "vertex.hpp"
#include <memory>
#include <vector>
#include <GLES3/gl3.h>

namespace jadeite
{
class Font;

struct TextBatch
{
	GLuint offset{ 0 };
	GLuint numVertices{ 0 };
	GLuint fontAtlasID{ 0 };
};

struct TextGlyph
{
	std::string sTextStr{ "" };
	glm::vec2 position{ 0.f };
	Color color{ 255, 255, 255, 255 };
	glm::mat4 model{ 1.f };
	std::shared_ptr<Font> pFont{ nullptr };
};

class TextBatchRenderer
{
public:
	TextBatchRenderer();
	~TextBatchRenderer();
	
	void Begin();
	void End(); 
	
	void AddText(const std::string& text, const std::shared_ptr<Font>& pFont, const glm::vec2& position,
				 const Color& color = Color{ 255, 255, 255, 255 }, const glm::mat4& model = glm::mat4{ 1.f } );
				
	void Render();
	
private:
	void Init();
	void CreateBatches();
	
private:
	GLuint m_VAO;
	GLuint m_VBO;
	
	std::vector<std::unique_ptr<TextGlyph>> m_TextGlyphs;
	std::vector<std::unique_ptr<TextBatch>> m_Batches;
};

} // jadeite
