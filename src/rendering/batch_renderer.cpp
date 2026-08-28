#include "batch_renderer.hpp"
#include <algorithm>

namespace jadeite
{

constexpr size_t NUM_SPRITE_VERTICES = 4;
constexpr size_t NUM_SPRITE_INDICES = 6;
constexpr size_t MAX_SPRITES = 10000;
constexpr size_t MAX_INDICES = MAX_SPRITES * NUM_SPRITE_INDICES;
constexpr size_t MAX_VERTICES = MAX_SPRITES * NUM_SPRITE_VERTICES;

BatchRenderer::BatchRenderer()
	: m_VAO{ 0 }
	, m_VBO{ 0 }
	, m_IBO{ 0 }
	, m_SpriteGlyphs{}
	, m_Batches{}
{
	Init();
}

BatchRenderer::~BatchRenderer()
{
	if (m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
	}
	if (m_VBO)
	{
		glDeleteVertexArrays(1, &m_VBO);
	}
	if (m_IBO)
	{
		glDeleteVertexArrays(1, &m_IBO);
	}
}

void BatchRenderer::Begin()
{
	m_Batches.clear();
	m_SpriteGlyphs.clear();
}

void BatchRenderer::End()
{
	if (m_SpriteGlyphs.empty())
		return;
	
	std::ranges::stable_sort(m_SpriteGlyphs, 
		[](const auto& a, const auto& b) { return a->layer < b->layer; } 
	);
	
	CreateBatches();
}

void BatchRenderer::AddSprite(const glm::vec4& destRect, const glm::vec4& uvRect,
	int layer, GLuint textureID, const Color& color, glm::mat4 model
)
{
	m_SpriteGlyphs.emplace_back(
		std::make_unique<SpriteGlyph>(
			SpriteGlyph{
				.topLeft = Vertex{
					.position = model * glm::vec4{ destRect.x, destRect.y + destRect.w, 0.f, 1.f },
					.uvs = UV{ .u = uvRect.x, .v = uvRect.y + uvRect.w },
					.color = color
				},
				.bottomLeft = Vertex{
					.position = model * glm::vec4{ destRect.x, destRect.y, 0.f, 1.f },
					.uvs = UV{ .u = uvRect.x, .v = uvRect.y },
					.color = color
				},
				.topRight = Vertex{
					.position = model * glm::vec4{ destRect.x + destRect.z, destRect.y + destRect.w, 0.f, 1.f },
					.uvs = UV{ .u = uvRect.x + uvRect.z, .v = uvRect.y + uvRect.w },
					.color = color
				},
				.bottomRight = Vertex{
					.position = model * glm::vec4{ destRect.x + destRect.z, destRect.y, 0.f, 1.f },
					.uvs = UV{ .u = uvRect.x + uvRect.z, .v = uvRect.y },
					.color = color
				},
				.layer = layer,
				.textureID = textureID
			}
		)
	);
}

void BatchRenderer::Render()
{
	if (m_Batches.empty())
		return;
	
	glBindVertexArray(m_VAO);
	
	for (const auto& batch : m_Batches)
	{
		glBindTexture(GL_TEXTURE_2D, batch->textureID);
		glDrawElements(
			GL_TRIANGLES,
			batch->numIndices,
			GL_UNSIGNED_INT,
			reinterpret_cast<void*>(sizeof(GLuint) * batch->offset)		
		);
	}
	
	// ADDED
	glBindVertexArray(0);
}

void BatchRenderer::CreateBatches()
{
	std::vector<Vertex> vertices;
	vertices.resize(m_SpriteGlyphs.size() * NUM_SPRITE_VERTICES);
	
	int currentIndex{ 0 };
	int currentSprite{ 0 };
	GLuint offset{ 0 };
	GLuint prevTextureID{ 0 };
	
	for (const auto& sprite : m_SpriteGlyphs)
	{
		if (currentSprite == 0)
		{
			m_Batches.emplace_back(std::make_unique<Batch>(
				Batch{ .numIndices = NUM_SPRITE_INDICES, .textureID = sprite->textureID, .offset = offset } )
			);
		}
		else if (sprite->textureID != prevTextureID)
		{
			m_Batches.emplace_back(std::make_unique<Batch>(
				Batch{ .numIndices = NUM_SPRITE_INDICES, .textureID = sprite->textureID, .offset = offset } )
			);
		}
		else
		{
			m_Batches.back()->numIndices += NUM_SPRITE_INDICES;
		}
		
		vertices[currentIndex++] = sprite->topLeft;
		vertices[currentIndex++] = sprite->topRight;
		vertices[currentIndex++] = sprite->bottomRight;
		vertices[currentIndex++] = sprite->bottomLeft;
		
		prevTextureID = sprite->textureID;
		offset += NUM_SPRITE_INDICES;
		currentSprite++;
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	
	// ADDED 
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchRenderer::Init()
{
	m_SpriteGlyphs.reserve(MAX_SPRITES);
	
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(
		0,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, position))
	);
	glEnableVertexAttribArray(0);
	 
	glVertexAttribPointer(
		1,
		2, 
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, uvs))
	);
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(
		2,
		4, 
		GL_UNSIGNED_BYTE,
		GL_TRUE,
		sizeof(Vertex),
		reinterpret_cast<void*>(offsetof(Vertex, color))
	);
	glEnableVertexAttribArray(2);
	
	GLuint offset{ 0 };
	
	GLuint indices[NUM_SPRITE_INDICES]{ 0, 1, 2, 2, 3, 0};
	
	auto indicesArr = std::make_unique<GLuint[]>(MAX_INDICES);
	
	for (size_t i = 0; i < MAX_INDICES; i += NUM_SPRITE_INDICES)
	{
		for (size_t j = 0; j < NUM_SPRITE_INDICES; j++)
			indicesArr[i + j] = indices[j] + offset;
			
		offset += NUM_SPRITE_VERTICES;
	}
	
	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		sizeof(GLuint) * MAX_INDICES,
		indicesArr.get(),
		GL_STATIC_DRAW
	);
	
	//ADDED
	glBindVertexArray(0);
}

} // jadeite 
