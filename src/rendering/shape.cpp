#include "shape.hpp"
#include "shape_batch_renderer.hpp"
#include "ecs/registry.hpp"
#include <cassert>

namespace jadeite
{
	
Rect::Rect(const glm::vec2& pos, const glm::vec2& sz, const Color& col, bool bWire)
	: position{ pos }
	, size{ sz }
	, color{ col }
	, bWireframe{ bWire }
{
}
	
void Rect::submit(ShapeRenderer& renderer) const 
{
	if ( bWireframe )
	{
		renderer.AddWireRectangle( position, size, color );
	}
	else
	{
		renderer.AddRectangle( position, size, color );
	}
}

Circle::Circle(const glm::vec2& center, float rad, const Color& col, int segments, bool bWire )
	: center{ center }
	, radius{ rad }
	, color{ col }
	, segments{ segments }
	, bWireframe{ bWire }
{
	
}

void Circle::submit(ShapeRenderer& renderer) const
{
	if ( bWireframe )
	{
		renderer.AddWireCircle( center, radius, color, segments );
	}
	else
	{
		renderer.AddCircle( center, radius, color, segments );
	}
}

Triangle::Triangle(const glm::vec2& pos, float b, float h, const Color& col, bool bWire)
	: position{ pos }
	, base{ b }
	, height{ h }
	, color{ col }
	, bWireframe{ bWire }
{
	
}

void Triangle::submit(ShapeRenderer& renderer) const
{
	
	glm::vec2 p1 = { position.x, position.y };
	glm::vec2 p2 = { position.x + base, position.y };
	glm::vec2 p3 = { position.x + base * 0.5f, position.y - height };
	
	if ( bWireframe )
	{
		renderer.AddWireTriangle( p1, p2, p3, color );
	}
	else
	{
		renderer.AddTriangle( p1, p2, p3, color );
	}
}

Polygon::Polygon(std::vector<glm::vec2> pts, const Color& col, bool bWire)
	: points{ std::move(pts) }
	, color{ col }
	, bWireframe{ bWire }
{
	
}

void Polygon::submit(ShapeRenderer& renderer) const 
{
	if ( bWireframe )
	{
		renderer.AddWirePolygon( points, color );
	}
	else
	{
		renderer.AddPolygon( points, color );
	}
}

Line::Line( std::array<glm::vec2, 2> pts, const Color& col )
	: points(std::move(pts))
	, color{ col }
{
}

void Line::submit(ShapeRenderer& renderer) const
{
	renderer.AddLine( points[0], points[1], color );
}


void ShapeBinder::CreateLuaBind(sol::state& lua, Registry& registry)
{
	lua.new_usertype<Rect>(
		"Rect",
		sol::call_constructor,
		sol::constructors<Rect(const glm::vec2&, const glm::vec2&, const Color&, bool)>(),
		"position", &Rect::position,
		"size", &Rect::size,
		"color", &Rect::color,
		"bWireframe", &Rect::bWireframe
	);
	
	lua.new_usertype<Circle>(
		"Circle",
		sol::call_constructor,
		sol::constructors<Circle(const glm::vec2&, float, const Color&, int, bool)>(),
		"center", &Circle::center,
		"radius", &Circle::radius,
		"segments", &Circle::segments,
		"color", &Circle::color,
		"bWireframe", &Circle::bWireframe
	);
	
	lua.new_usertype<Triangle>(
		"Triangle",
		sol::call_constructor,
		sol::constructors<Triangle(const glm::vec2&, float, float, const Color&, bool)>(),
		"position", &Triangle::position,
		"base", &Triangle::base,
		"height", &Triangle::height,
		"color", &Triangle::color,
		"bWireframe", &Triangle::bWireframe
	);
	
	lua.new_usertype<Polygon>(
		"Polygon",
		sol::call_constructor,
		sol::factories(
			[](const sol::table& pts, const Color& color, bool bWireframe)
			{
				std::vector<glm::vec2> points{};
				for (const auto& [_, object] : pts)
				{
					const auto& point = object.as<glm::vec2>();
					points.push_back(point);
				}
				
				return Polygon{ points, color, bWireframe };
			}
		),		
		"points", &Polygon::points,
		"color", &Polygon::color,
		"bWireframe", &Polygon::bWireframe
	);
	
	lua.new_usertype<Line>(
		"Line",
		sol::call_constructor,
		sol::factories(
			[](const glm::vec2& p1, const glm::vec2& p2, const Color& color)
			{
				std::array<glm::vec2, 2> points{ p1, p2 };
				return Line{ points, color };
			}
		),
		"points", &Line::points,
		"color", &Line::color
	);
	
	auto& pShapeRenderer = registry.GetContext<std::shared_ptr<ShapeRenderer>>();
	assert(pShapeRenderer && "Shape Renderer was not added to the registry context!");
		
	lua.set_function("DrawRect", 
		[pShapeRenderer](const Rect& rect) 
		{ 
			rect.submit(*pShapeRenderer);
		}
	);
	
	// Raw-number fast path for hot loops (tilemaps, grids, etc). Takes plain
	// floats/ints instead of vec2/Color/Rect userdata, so sol2 only has to
	// pass primitives - no usertype construction, no Rect object at all.
	// This is 1 Lua->C++ crossing per call instead of the 3 DrawRect(Rect(
	// vec2(...), vec2(...), Color(...))) costs.
	lua.set_function("DrawRectFast",
		[pShapeRenderer](float x, float y, float w, float h, uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool bWireframe)
		{
			if (bWireframe)
			{
				pShapeRenderer->AddWireRectangle(glm::vec2{ x, y }, glm::vec2{ w, h }, Color{ r, g, b, a });
			}
			else
			{
				pShapeRenderer->AddRectangle(glm::vec2{ x, y }, glm::vec2{ w, h }, Color{ r, g, b, a });
			}
		}
	);
	
	lua.set_function("DrawTriangle", 
		[pShapeRenderer](const Triangle& triangle) 
		{ 
			triangle.submit(*pShapeRenderer);
		}
	);
	
	lua.set_function("DrawCircle", 
		[pShapeRenderer](const Circle& circle) 
		{ 
			circle.submit(*pShapeRenderer);
		}
	);
	
	lua.set_function("DrawPolygon", 
		[pShapeRenderer](const Polygon& polygon) 
		{ 
			polygon.submit(*pShapeRenderer);
		}
	);
	
	lua.set_function("DrawLine", 
		[pShapeRenderer](const Line& line) 
		{ 
			line.submit(*pShapeRenderer);
		}
	);
}
	
} // jadeite
