#pragma once
#include <GLES3/gl3.h>
#include <glm/glm.hpp>
#include <vector>

#include "vertex.hpp"

namespace jadeite
{
	// Only what CreateBatches needs to reorder: which slice of the index
	// buffer this shape owns, and which layer it belongs to. Vertex data
	// never moves, so no vertex offset/count bookkeeping is needed here.
	struct ShapeGlyph
	{
		int layer{ 0 };

		unsigned int indexOffset{ 0 };
		unsigned int indexCount{ 0 };
	};

	struct ShapeBatch
	{
		unsigned int offset{ 0 };
		unsigned int numIndices{ 0 };
		int layer{ 0 };
	};

	struct LineGlyph
	{
		int layer{ 0 };

		unsigned int indexOffset{ 0 };
		unsigned int indexCount{ 0 };
	};

	struct LineBatch
	{
		unsigned int offset{ 0 };
		unsigned int numIndices{ 0 };
		int layer{ 0 };
	};

	class ShapeRenderer
	{
	public:
		ShapeRenderer();
		~ShapeRenderer();

		void Begin();
		void End();
		void Render();

		// Solid Shapes
		void AddRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer = 0);
		void AddTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const Color& color, int layer = 0);
		void AddCircle(const glm::vec2& center, float radius, const Color& color, int segments = 32, int layer = 0);
		void AddPolygon(const std::vector<glm::vec2>& points, const Color& color, int layer = 0);
		void AddLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, int layer = 0);

		// Wireframe shapes
		void AddWireRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer = 0);
		void AddWireTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const Color& color, int layer = 0);
		void AddWireCircle(const glm::vec2& center, float radius, const Color& color, int segments = 32, int layer = 0);
		void AddWirePolygon(const std::vector<glm::vec2>& points, const Color& color, int layer = 0);

	private:
		void Initialize();
		void CreateBatches();
		void CreateSolidBatches();
		void CreateLineBatches();

		// Safety valve only. Fires if a single frame's shapes exceed the
		// entire preallocated scene budget, which normal use should never
		// hit. Sorts, batches, draws immediately, resets scene counters.
		void FlushSolid();
		void FlushLines();

	private:
		GLuint m_VAO;
		GLuint m_VBO;
		GLuint m_EBO;

		GLuint m_LineVAO;
		GLuint m_LineVBO;
		GLuint m_LineEBO;

		std::vector<ShapeVertex> m_Vertices;
		std::vector<unsigned int> m_Indices;
		unsigned int m_VertexCount;
		unsigned int m_IndexCount;

		std::vector<unsigned int> m_SortedIndices;
		unsigned int m_SortedIndexCount;

		std::vector<ShapeGlyph> m_ShapeGlyphs;
		std::vector<ShapeBatch> m_Batches;
		unsigned int m_ShapeGlyphCount;
		unsigned int m_BatchCount;

		std::vector<ShapeVertex> m_LineVertices;
		std::vector<unsigned int> m_LineIndices;
		unsigned int m_LineVertexCount;
		unsigned int m_LineIndexCount;

		std::vector<unsigned int> m_SortedLineIndices;
		unsigned int m_SortedLineIndexCount;

		std::vector<LineGlyph> m_LineGlyphs;
		std::vector<LineBatch> m_LineBatches;
		unsigned int m_LineGlyphCount;
		unsigned int m_LineBatchCount;
	};

} // jadeite
