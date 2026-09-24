#include "text_batch_renderer.hpp"
#include "font.hpp"

namespace jadeite
{

/* Vertice variables */
constexpr int MAX_VERTICES = 24000;
constexpr int NUM_VERTICES = 6;

/* Helper function to parse the text component string so that it accepts \n and does a new line */
std::vector<std::string> splitNewlines(const std::string& text) 
{
    std::vector<std::string> tokens;
    std::string delimiter = "\n";
    size_t start = 0;
    size_t end = text.find(delimiter);

    while (end != std::string::npos) {
        tokens.push_back(text.substr(start, end - start));
        start = end + delimiter.length();
        end = text.find(delimiter, start);
    }
  
    tokens.push_back(text.substr(start));
    return tokens;
}
	
/* Default constructor */
TextBatchRenderer::TextBatchRenderer()
	: m_VAO{ 0 }
	, m_VBO{ 0 }
	, m_TextGlyphs{}
	, m_Batches{}
{
	Init();
}

/* Destructor */
TextBatchRenderer::~TextBatchRenderer()
{
	glDeleteVertexArrays( 1, &m_VAO );
	glDeleteBuffers( 1, &m_VBO );
}

/* Called before you begin the batching. It clears the batches and text glyphs */
void TextBatchRenderer::Begin()
{
	m_Batches.clear();
	m_TextGlyphs.clear();
}

/* Creates the batches */
void TextBatchRenderer::End()
{
	if (m_TextGlyphs.empty())
		return;
	
	CreateBatches();
}



/* Adds text requiring a string to display, font pointer, position, color, and mat4 model */
void TextBatchRenderer::AddText(const std::string& text, const std::shared_ptr<Font>& pFont, const glm::vec2& position,
			 const Color& color, const glm::mat4& model)
{	
	auto parsed = splitNewlines(text);
	// looped for each /n detected and placed the text to the next line by the font size
	for(int i = 0; i < parsed.size(); i++)
	{
		m_TextGlyphs.emplace_back( 
			std::make_unique<TextGlyph>(
				TextGlyph{
					.sTextStr = parsed[i],
					.position = position + glm::vec2(0, pFont->GetFontSize() * i),
					.color = color,
					.model = model,
					.pFont = pFont 
				}
			)
		);
	}
}

/* Renders the actual batches */
void TextBatchRenderer::Render()
{
	if (m_Batches.empty())
		return;
	
	glBindVertexArray(m_VAO);
	for (const auto& batch : m_Batches)
	{
		glBindTexture(GL_TEXTURE_2D, batch->fontAtlasID);
		glDrawArrays(GL_TRIANGLES, batch->offset, batch->numVertices);
	}
}

/* Initializes the text batch renderer */
void TextBatchRenderer::Init()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	glBufferData(GL_ARRAY_BUFFER, MAX_VERTICES * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, position)));
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, uvs)));
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, color)));
	glEnableVertexAttribArray(2);
	
	glBindVertexArray(0);
}	

/* Creates the batch to be rendered */
void TextBatchRenderer::CreateBatches()
{
	GLuint offset{ 0 };
	GLuint prevFontID{ 0 };
	
	int currentFont{ 0 };
	
	size_t total{ 0 };
	size_t currentVertex{ 0 };
	
	for (const auto& textGlyph : m_TextGlyphs)
		total += textGlyph->sTextStr.size();
	
	std::vector<Vertex> vertices;
	vertices.resize(total * NUM_VERTICES);
	
	for (const auto& textGlyph : m_TextGlyphs)
	{
		glm::vec2 tempPos = textGlyph->position;
		
		for (const auto& character : textGlyph->sTextStr)
		{
			auto glyph = textGlyph->pFont->GetGlyph(character, tempPos);
			
			// First Triangle
			vertices[currentVertex++] = Vertex {
				.position = textGlyph->model * glm::vec4{ glyph.min.position.x, glyph.min.position.y, 0.f, 1.f },
				.uvs = UV{ .u = glyph.min.uvs.u, .v = glyph.min.uvs.v },
				.color = textGlyph->color
			};
			
			vertices[currentVertex++] = Vertex {
				.position = textGlyph->model * glm::vec4{ glyph.max.position.x, glyph.min.position.y, 0.f, 1.f },
				.uvs = UV{ .u = glyph.max.uvs.u, .v = glyph.min.uvs.v },
				.color = textGlyph->color
			};
			
			vertices[currentVertex++] = Vertex {
				.position = textGlyph->model * glm::vec4{ glyph.max.position.x, glyph.max.position.y, 0.f, 1.f },
				.uvs = UV{ .u = glyph.max.uvs.u, .v = glyph.max.uvs.v },
				.color = textGlyph->color
			};
			
			// Second Triangle
			vertices[currentVertex++] = Vertex {
				.position = textGlyph->model * glm::vec4{ glyph.min.position.x, glyph.min.position.y, 0.f, 1.f },
				.uvs = UV{ .u = glyph.min.uvs.u, .v = glyph.min.uvs.v },
				.color = textGlyph->color
			};
			
			vertices[currentVertex++] = Vertex {
				.position = textGlyph->model * glm::vec4{ glyph.max.position.x, glyph.max.position.y, 0.f, 1.f },
				.uvs = UV{ .u = glyph.max.uvs.u, .v = glyph.max.uvs.v },
				.color = textGlyph->color
			};
			
			vertices[currentVertex++] = Vertex {
				.position = textGlyph->model * glm::vec4{ glyph.min.position.x, glyph.max.position.y, 0.f, 1.f },
				.uvs = UV{ .u = glyph.min.uvs.u, .v = glyph.max.uvs.v },
				.color = textGlyph->color
			};
			
			if ( currentFont == 0 )
			{
				m_Batches.emplace_back(
					std::make_unique<TextBatch>(
						TextBatch{ 
							.offset = offset, 
							.numVertices = NUM_VERTICES, 
							.fontAtlasID = textGlyph->pFont->GetFontAtlasID()
						}
					)
				);
			}
			else if (textGlyph->pFont->GetFontAtlasID() != prevFontID)
			{
				m_Batches.emplace_back(
					std::make_unique<TextBatch>(
						TextBatch{ 
							.offset = offset, 
							.numVertices = NUM_VERTICES, 
							.fontAtlasID = textGlyph->pFont->GetFontAtlasID()
						}
					)
				);
			}
			else
			{
				m_Batches.back()->numVertices += NUM_VERTICES;
			}
			
			currentFont++;
			prevFontID = textGlyph->pFont->GetFontAtlasID();
			offset += NUM_VERTICES;
		}
	}
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

} // jadeite::TextBatchRenderer