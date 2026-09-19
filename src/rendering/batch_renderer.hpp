#pragma once
#include "vertex.hpp"
#include <vector>
#include <memory>
#include <GLES3/gl3.h>

namespace jadeite
{
	
struct Batch
{
	GLuint numIndices{ 0 };
	GLuint textureID{ 0 };
	GLuint offset{ 0 };
};

struct SpriteGlyph
{
	Vertex topLeft;
	Vertex bottomLeft;
	Vertex topRight;
	Vertex bottomRight;
	
	int layer;
	GLuint textureID{ 0 };
	
	bool bFlipX{ false };
	bool bFlipY{ false };
};

struct SpriteGlyphParams
{
	glm::vec4 destRect{ 0.f };
	glm::vec4 uvRect{ 0.f };
	int layer{ 1 };
	GLuint textureID{ 0 };
	Color color{255, 255, 255, 255};
	glm::mat4 model{ 1.f };
	bool bFlipX{ false };
	bool bFlipY{ false };
};

class BatchRenderer
{
public:
	BatchRenderer();
	~BatchRenderer();
	
	void Begin();
	void End();
	
	void AddSprite(const glm::vec4& destRect, const glm::vec4& uvRect,
		int layer, GLuint textureID, const Color& color, glm::mat4 model = glm::mat4{1.f} );
	
	void AddSprite(const SpriteGlyphParams& params);
	
	void Render();
	
private:
	void CreateBatches();
	void Init();
	
private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
	
	std::vector<std::unique_ptr<SpriteGlyph>> m_SpriteGlyphs;
	std::vector<std::unique_ptr<Batch>> m_Batches;
};

} // jadeite
