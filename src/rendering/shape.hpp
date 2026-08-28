#pragma once 
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "vertex.hpp"

namespace jadeite
{
	
class ShapeRenderer;

struct IShape
{
	virtual void submit(ShapeRenderer& renderer) const = 0;
	virtual ~IShape() = default;
};

struct Rect : public IShape
{
	glm::vec2 position;
	glm::vec2 size;
	Color color;
	bool bWireframe;
	
	Rect(const glm::vec2& pos, const glm::vec2& sz, const Color& col = Color{}, bool bWire = false );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

struct Circle : public IShape
{
	glm::vec2 center;
	float radius;
	Color color;
	int segments;
	bool bWireframe;
	
	Circle(const glm::vec2& center, float rad, const Color& col = Color{}, int segments = 32, bool bWire = false );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

struct Triangle : public IShape
{
	glm::vec2 position;
	float base;
	float height;
	Color color;
	bool bWireframe;
	
	Triangle(const glm::vec2& pos, float b, float h, const Color& col = Color{}, bool bWire = false );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

struct Polygon : public IShape
{
	std::vector<glm::vec2> points;
	Color color;
	bool bWireframe;
	
	Polygon(std::vector<glm::vec2> pts, const Color& col = Color{}, bool bWire = false );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

struct Line : public IShape
{
	std::array<glm::vec2, 2> points;
	Color color;

	Line( std::array<glm::vec2, 2> pts, const Color& col = Color{} );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

struct ShapeBinder
{
	static void CreateLuaBind(sol::state& lua, class Registry& registry);
};

} // jadeite
