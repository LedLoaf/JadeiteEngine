#include "shape_batch_renderer.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>

namespace jadeite
{
	constexpr float PI = 3.1415926f;
	constexpr float TWOPI = PI * 2.f;

	constexpr unsigned int MAX_SCENE_VERTICES = 300000;
	constexpr unsigned int MAX_SCENE_INDICES = 450000;
	constexpr unsigned int MAX_SCENE_LINE_VERTICES = 40000;

	constexpr unsigned int MAX_GLYPHS = 70000;
	constexpr unsigned int MAX_BATCHES = MAX_GLYPHS;

	constexpr unsigned int MAX_LINE_GLYPHS = 8192;
	constexpr unsigned int MAX_LINE_BATCHES = MAX_LINE_GLYPHS;

	ShapeRenderer::ShapeRenderer()
		: m_VAO{ 0 }
		, m_VBO{ 0 }
		, m_EBO{ 0 }
		, m_LineVAO{ 0 }
		, m_LineVBO{ 0 }
		, m_LineEBO{ 0 }
		, m_Vertices{}
		, m_Indices{}
		, m_VertexCount{ 0 }
		, m_IndexCount{ 0 }
		, m_SortedIndices{}
		, m_SortedIndexCount{ 0 }
		, m_ShapeGlyphs{}
		, m_Batches{}
		, m_ShapeGlyphCount{ 0 }
		, m_BatchCount{ 0 }
		, m_LineVertices{}
		, m_LineIndices{}
		, m_LineVertexCount{ 0 }
		, m_LineIndexCount{ 0 }
		, m_SortedLineIndices{}
		, m_SortedLineIndexCount{ 0 }
		, m_LineGlyphs{}
		, m_LineBatches{}
		, m_LineGlyphCount{ 0 }
		, m_LineBatchCount{ 0 }
	{
		Initialize();
	}

	ShapeRenderer::~ShapeRenderer()
	{
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_EBO);
		glDeleteVertexArrays(1, &m_VAO);

		glDeleteBuffers(1, &m_LineVBO);
		glDeleteBuffers(1, &m_LineEBO);
		glDeleteVertexArrays(1, &m_LineVAO);
	}

	void ShapeRenderer::Begin()
	{
		m_VertexCount = 0;
		m_IndexCount = 0;
		m_ShapeGlyphCount = 0;

		m_LineVertexCount = 0;
		m_LineIndexCount = 0;
		m_LineGlyphCount = 0;
	}

	void ShapeRenderer::End()
	{
		std::sort( m_ShapeGlyphs.begin(), m_ShapeGlyphs.begin() + m_ShapeGlyphCount,
			[]( const ShapeGlyph& a, const ShapeGlyph& b )
			{
				return a.layer < b.layer;
			});

		std::sort( m_LineGlyphs.begin(), m_LineGlyphs.begin() + m_LineGlyphCount,
			[]( const LineGlyph& a, const LineGlyph& b )
			{
				return a.layer < b.layer;
			});

		CreateBatches();
	}

	void ShapeRenderer::CreateBatches()
	{
		CreateSolidBatches();
		CreateLineBatches();
	}

	void ShapeRenderer::CreateSolidBatches()
	{
		if ( m_ShapeGlyphCount == 0 )
		{
			return;
		}

		m_BatchCount = 0;
		m_SortedIndexCount = 0;

		int currentLayer = m_ShapeGlyphs[ 0 ].layer;
		unsigned int batchStart = 0;

		for ( unsigned int g = 0; g < m_ShapeGlyphCount; ++g )
		{
			const ShapeGlyph& glyph = m_ShapeGlyphs[ g ];

			if ( glyph.layer != currentLayer )
			{
				unsigned int numIndices = m_SortedIndexCount - batchStart;
				if ( numIndices > 0 && m_BatchCount < m_Batches.size() )
				{
					m_Batches[ m_BatchCount ].offset = batchStart;
					m_Batches[ m_BatchCount ].numIndices = numIndices;
					m_Batches[ m_BatchCount ].layer = currentLayer;
					++m_BatchCount;
				}

				currentLayer = glyph.layer;
				batchStart = m_SortedIndexCount;
			}

			// Direct copy, no rebasing. Indices already reference absolute
			// positions in m_Vertices, which is uploaded whole, unsorted,
			// below - only the index order changes per frame.
			for ( unsigned int i = 0; i < glyph.indexCount; ++i )
			{
				m_SortedIndices[ m_SortedIndexCount++ ] = m_Indices[ glyph.indexOffset + i ];
			}
		}

		unsigned int numIndices = m_SortedIndexCount - batchStart;
		if ( numIndices > 0 && m_BatchCount < m_Batches.size() )
		{
			m_Batches[ m_BatchCount ].offset = batchStart;
			m_Batches[ m_BatchCount ].numIndices = numIndices;
			m_Batches[ m_BatchCount ].layer = currentLayer;
			++m_BatchCount;
		}

		// One upload for the whole frame's solid geometry. The vertex
		// buffer goes up exactly as written, no reordering needed.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_VertexCount * sizeof(ShapeVertex), m_Vertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_SortedIndexCount * sizeof(unsigned int), m_SortedIndices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ShapeRenderer::CreateLineBatches()
	{
		if ( m_LineGlyphCount == 0 )
		{
			return;
		}

		m_LineBatchCount = 0;
		m_SortedLineIndexCount = 0;

		int currentLayer = m_LineGlyphs[ 0 ].layer;
		unsigned int batchStart = 0;

		for ( unsigned int g = 0; g < m_LineGlyphCount; ++g )
		{
			const LineGlyph& glyph = m_LineGlyphs[ g ];

			if ( glyph.layer != currentLayer )
			{
				unsigned int numIndices = m_SortedLineIndexCount - batchStart;
				if ( numIndices > 0 && m_LineBatchCount < m_LineBatches.size() )
				{
					m_LineBatches[ m_LineBatchCount ].offset = batchStart;
					m_LineBatches[ m_LineBatchCount ].numIndices = numIndices;
					m_LineBatches[ m_LineBatchCount ].layer = currentLayer;
					++m_LineBatchCount;
				}

				currentLayer = glyph.layer;
				batchStart = m_SortedLineIndexCount;
			}

			for ( unsigned int i = 0; i < glyph.indexCount; ++i )
			{
				m_SortedLineIndices[ m_SortedLineIndexCount++ ] = m_LineIndices[ glyph.indexOffset + i ];
			}
		}

		unsigned int numIndices = m_SortedLineIndexCount - batchStart;
		if ( numIndices > 0 && m_LineBatchCount < m_LineBatches.size() )
		{
			m_LineBatches[ m_LineBatchCount ].offset = batchStart;
			m_LineBatches[ m_LineBatchCount ].numIndices = numIndices;
			m_LineBatches[ m_LineBatchCount ].layer = currentLayer;
			++m_LineBatchCount;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, m_LineVertexCount * sizeof(ShapeVertex), m_LineVertices.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineEBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, m_SortedLineIndexCount * sizeof(unsigned int), m_SortedLineIndices.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void ShapeRenderer::FlushSolid()
	{
		if ( m_ShapeGlyphCount == 0 )
		{
			return;
		}

		std::sort( m_ShapeGlyphs.begin(), m_ShapeGlyphs.begin() + m_ShapeGlyphCount,
			[]( const ShapeGlyph& a, const ShapeGlyph& b )
			{
				return a.layer < b.layer;
			});

		CreateSolidBatches();

		glBindVertexArray(m_VAO);
		for ( unsigned int b = 0; b < m_BatchCount; ++b )
		{
			const ShapeBatch& batch = m_Batches[ b ];
			glDrawElements(GL_TRIANGLES, batch.numIndices, GL_UNSIGNED_INT, (void*)( batch.offset * sizeof(unsigned int) ));
		}
		glBindVertexArray(0);

		m_BatchCount = 0;
		m_VertexCount = 0;
		m_IndexCount = 0;
		m_ShapeGlyphCount = 0;
	}

	void ShapeRenderer::FlushLines()
	{
		if ( m_LineGlyphCount == 0 )
		{
			return;
		}

		std::sort( m_LineGlyphs.begin(), m_LineGlyphs.begin() + m_LineGlyphCount,
			[]( const LineGlyph& a, const LineGlyph& b )
			{
				return a.layer < b.layer;
			});

		CreateLineBatches();

		glBindVertexArray(m_LineVAO);
		for ( unsigned int b = 0; b < m_LineBatchCount; ++b )
		{
			const LineBatch& batch = m_LineBatches[ b ];
			glDrawElements(GL_LINES, batch.numIndices, GL_UNSIGNED_INT, (void*)( batch.offset * sizeof(unsigned int) ));
		}
		glBindVertexArray(0);

		m_LineBatchCount = 0;
		m_LineVertexCount = 0;
		m_LineIndexCount = 0;
		m_LineGlyphCount = 0;
	}

	void ShapeRenderer::Render()
	{
		glBindVertexArray(m_VAO);
		for ( unsigned int b = 0; b < m_BatchCount; ++b )
		{
			const ShapeBatch& batch = m_Batches[ b ];
			glDrawElements(GL_TRIANGLES, batch.numIndices, GL_UNSIGNED_INT, (void*)( batch.offset * sizeof(unsigned int) ));
		}
		glBindVertexArray(0);

		glBindVertexArray(m_LineVAO);
		for ( unsigned int b = 0; b < m_LineBatchCount; ++b )
		{
			const LineBatch& batch = m_LineBatches[ b ];
			glDrawElements(GL_LINES, batch.numIndices, GL_UNSIGNED_INT, (void*)( batch.offset * sizeof(unsigned int) ));
		}
		glBindVertexArray(0);
	}

	// Solid Shapes
	void ShapeRenderer::AddRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer)
	{
		constexpr unsigned int vertsNeeded = 4;
		constexpr unsigned int indicesNeeded = 6;

		if ( m_ShapeGlyphCount >= m_ShapeGlyphs.size()
			|| m_VertexCount + vertsNeeded > m_Vertices.size()
			|| m_IndexCount + indicesNeeded > m_Indices.size() )
		{
			FlushSolid();
		}

		assert( m_ShapeGlyphCount < m_ShapeGlyphs.size()
			&& m_VertexCount + vertsNeeded <= m_Vertices.size()
			&& m_IndexCount + indicesNeeded <= m_Indices.size()
			&& "Single shape exceeds total ShapeRenderer capacity, raise MAX_SCENE_VERTICES / MAX_SCENE_INDICES / MAX_GLYPHS" );
		
		if ( m_ShapeGlyphCount >= m_ShapeGlyphs.size()
			|| m_VertexCount + vertsNeeded > m_Vertices.size()
			|| m_IndexCount + indicesNeeded > m_Indices.size() )
		{
			return;
		}

		unsigned int base = m_VertexCount;
		unsigned int indexOffset = m_IndexCount;

		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ position, color };								// TL
		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ position + glm::vec2{size.x, 0.f}, color };		// TR
		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ position + size, color };							// BR
		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ position + glm::vec2{0, size.y}, color };			// BL

		m_Indices[ m_IndexCount++ ] = base + 0;
		m_Indices[ m_IndexCount++ ] = base + 1;
		m_Indices[ m_IndexCount++ ] = base + 2;
		m_Indices[ m_IndexCount++ ] = base + 2;
		m_Indices[ m_IndexCount++ ] = base + 3;
		m_Indices[ m_IndexCount++ ] = base + 0;

		ShapeGlyph& glyph = m_ShapeGlyphs[ m_ShapeGlyphCount++ ];
		glyph.layer = layer;
		glyph.indexOffset = indexOffset;
		glyph.indexCount = indicesNeeded;
	}

	void ShapeRenderer::AddTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const Color& color, int layer)
	{
		constexpr unsigned int vertsNeeded = 3;
		constexpr unsigned int indicesNeeded = 3;

		if ( m_ShapeGlyphCount >= m_ShapeGlyphs.size()
			|| m_VertexCount + vertsNeeded > m_Vertices.size()
			|| m_IndexCount + indicesNeeded > m_Indices.size() )
		{
			FlushSolid();
		}

		unsigned int base = m_VertexCount;
		unsigned int indexOffset = m_IndexCount;

		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ p1, color };
		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ p2, color };
		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ p3, color };

		m_Indices[ m_IndexCount++ ] = base;
		m_Indices[ m_IndexCount++ ] = base + 1;
		m_Indices[ m_IndexCount++ ] = base + 2;

		ShapeGlyph& glyph = m_ShapeGlyphs[ m_ShapeGlyphCount++ ];
		glyph.layer = layer;
		glyph.indexOffset = indexOffset;
		glyph.indexCount = indicesNeeded;
	}

	void ShapeRenderer::AddCircle(const glm::vec2& center, float radius, const Color& color, int segments, int layer)
	{
		// center vertex + segments ring vertices. The last triangle closes
		// back to the first ring vertex via modulo, no duplicate seam vertex.
		unsigned int vertsNeeded = static_cast<unsigned int>( segments ) + 1;
		unsigned int indicesNeeded = static_cast<unsigned int>( segments ) * 3;

		if ( m_ShapeGlyphCount >= m_ShapeGlyphs.size()
			|| m_VertexCount + vertsNeeded > m_Vertices.size()
			|| m_IndexCount + indicesNeeded > m_Indices.size() )
		{
			FlushSolid();
		}

		unsigned int centerIndex = m_VertexCount;
		unsigned int indexOffset = m_IndexCount;

		m_Vertices[ m_VertexCount++ ] = ShapeVertex{ center, color };

		float fSegments{ static_cast<float>(segments) };
		for ( int j = 0; j < segments; j++ )
		{
			float angle = (j / fSegments) * TWOPI;
			glm::vec2 offset = { cos(angle) * radius, sin(angle) * radius };
			m_Vertices[ m_VertexCount++ ] = ShapeVertex{ center + offset, color };
		}

		for ( int j = 1; j <= segments; j++ )
		{
			m_Indices[ m_IndexCount++ ] = centerIndex;
			m_Indices[ m_IndexCount++ ] = centerIndex + j;
			m_Indices[ m_IndexCount++ ] = centerIndex + (j % segments) + 1;
		}

		ShapeGlyph& glyph = m_ShapeGlyphs[ m_ShapeGlyphCount++ ];
		glyph.layer = layer;
		glyph.indexOffset = indexOffset;
		glyph.indexCount = indicesNeeded;
	}

	void ShapeRenderer::AddPolygon(const std::vector<glm::vec2>& points, const Color& color, int layer)
	{
		if ( points.size() < 3 )
		{
			return;
		}

		unsigned int vertsNeeded = static_cast<unsigned int>( points.size() );
		unsigned int indicesNeeded = static_cast<unsigned int>( (points.size() - 2) * 3 );

		if ( m_ShapeGlyphCount >= m_ShapeGlyphs.size()
			|| m_VertexCount + vertsNeeded > m_Vertices.size()
			|| m_IndexCount + indicesNeeded > m_Indices.size() )
		{
			FlushSolid();
		}
		
		unsigned int base = m_VertexCount;
		unsigned int indexOffset = m_IndexCount;

		for ( const auto& point : points )
		{
			m_Vertices[ m_VertexCount++ ] = ShapeVertex{ point, color };
		}

		for ( size_t j = 1; j < points.size() - 1; j++ )
		{
			m_Indices[ m_IndexCount++ ] = base;
			m_Indices[ m_IndexCount++ ] = static_cast<unsigned int>( base + j );
			m_Indices[ m_IndexCount++ ] = static_cast<unsigned int>( base + j + 1 );
		}

		ShapeGlyph& glyph = m_ShapeGlyphs[ m_ShapeGlyphCount++ ];
		glyph.layer = layer;
		glyph.indexOffset = indexOffset;
		glyph.indexCount = indicesNeeded;
	}

	void ShapeRenderer::AddLine(const glm::vec2& p1, const glm::vec2& p2, const Color& color, int layer)
	{
		constexpr unsigned int vertsNeeded = 2;
		constexpr unsigned int indicesNeeded = 2;

		if ( m_LineGlyphCount >= m_LineGlyphs.size()
			|| m_LineVertexCount + vertsNeeded > m_LineVertices.size()
			|| m_LineIndexCount + indicesNeeded > m_LineIndices.size() )
		{
			FlushLines();
		}

		unsigned int base = m_LineVertexCount;
		unsigned int indexOffset = m_LineIndexCount;

		m_LineVertices[ m_LineVertexCount++ ] = ShapeVertex{ p1, color };
		m_LineVertices[ m_LineVertexCount++ ] = ShapeVertex{ p2, color };

		m_LineIndices[ m_LineIndexCount++ ] = base;
		m_LineIndices[ m_LineIndexCount++ ] = base + 1;

		LineGlyph& glyph = m_LineGlyphs[ m_LineGlyphCount++ ];
		glyph.layer = layer;
		glyph.indexOffset = indexOffset;
		glyph.indexCount = indicesNeeded;
	}

	// Wireframe shapes
	void ShapeRenderer::AddWireRectangle(const glm::vec2& position, const glm::vec2& size, const Color& color, int layer)
	{
		glm::vec2 p1 = position;
		glm::vec2 p2 = position + glm::vec2{size.x, 0 };
		glm::vec2 p3 = position + size;
		glm::vec2 p4 = position + glm::vec2{0, size.y };

		AddLine(p1, p2, color, layer);
		AddLine(p2, p3, color, layer);
		AddLine(p3, p4, color, layer);
		AddLine(p4, p1, color, layer);
	}

	void ShapeRenderer::AddWireTriangle(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& p3, const Color& color, int layer)
	{
		AddLine(p1, p2, color, layer);
		AddLine(p2, p3, color, layer);
		AddLine(p3, p1, color, layer);
	}

	void ShapeRenderer::AddWireCircle(const glm::vec2& center, float radius, const Color& color, int segments, int layer)
	{
		float angleStep = TWOPI / segments;
		for ( int i = 0; i < segments; ++i )
		{
			float a0 = angleStep * i;
			float a1 = angleStep * (i + 1);

			glm::vec2 p0 = center + radius * glm::vec2{ std::cos(a0), std::sin(a0) };
			glm::vec2 p1 = center + radius * glm::vec2{ std::cos(a1), std::sin(a1) };

			AddLine( p0, p1, color, layer );
		}
	}

	void ShapeRenderer::AddWirePolygon(const std::vector<glm::vec2>& points, const Color& color, int layer)
	{
		size_t n = points.size();
		for ( size_t i = 0; i < n; ++i )
		{
			AddLine(points[i], points[(i + 1) % n], color, layer);
		}
	}

	void ShapeRenderer::Initialize()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_SCENE_VERTICES * sizeof(ShapeVertex), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_SCENE_INDICES * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, position) );
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, color));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenVertexArrays(1, &m_LineVAO);
		glGenBuffers(1, &m_LineVBO);
		glGenBuffers(1, &m_LineEBO);

		glBindVertexArray(m_LineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_LineVBO);
		glBufferData(GL_ARRAY_BUFFER, MAX_SCENE_LINE_VERTICES * sizeof(ShapeVertex), nullptr, GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, MAX_SCENE_LINE_VERTICES * sizeof(unsigned int), nullptr, GL_DYNAMIC_DRAW);

		// Position
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, position) );
		glEnableVertexAttribArray(0);

		// Color
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ShapeVertex), (void*)offsetof(ShapeVertex, color));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		// Persistent CPU-side storage, sized exactly once, right here.
		// Nothing below this point ever calls resize / reserve again.
		m_Vertices.resize( MAX_SCENE_VERTICES );
		m_Indices.resize( MAX_SCENE_INDICES );
		m_SortedIndices.resize( MAX_SCENE_INDICES );

		m_ShapeGlyphs.resize( MAX_GLYPHS );
		m_Batches.resize( MAX_BATCHES );

		m_LineVertices.resize( MAX_SCENE_LINE_VERTICES );
		m_LineIndices.resize( MAX_SCENE_LINE_VERTICES );
		m_SortedLineIndices.resize( MAX_SCENE_LINE_VERTICES );

		m_LineGlyphs.resize( MAX_LINE_GLYPHS );
		m_LineBatches.resize( MAX_LINE_BATCHES );
	}

} // jadeite
