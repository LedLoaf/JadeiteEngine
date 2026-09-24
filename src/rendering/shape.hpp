#pragma once 
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <sol/sol.hpp>

#include "vertex.hpp"

namespace jadeite
{
	
class ShapeRenderer;

/* Holds the interface for a shape */
struct IShape
{
	virtual void submit(ShapeRenderer& renderer) const = 0;
	virtual ~IShape() = default;
};

/* Rectangle shape struct keeping track of the position, size, color, and a wire frame flag */
struct Rect : public IShape
{
	glm::vec2 position;
	glm::vec2 size;
	Color color;
	bool bWireframe;
	
	Rect(const glm::vec2& pos, const glm::vec2& sz, const Color& col = Color{}, bool bWire = false );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

/* Circle shape struct keeping track of the center position, radius, color, segments, and a wire frame flag */
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

/* Triangle shape struct keeping track of the position, base, height, color, and a wire frame flag */
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

/*Polygon shape struct keeping track of the points, color, and a wire frame flag*/
struct Polygon : public IShape
{
	std::vector<glm::vec2> points;
	Color color;
	bool bWireframe;
	
	Polygon(std::vector<glm::vec2> pts, const Color& col = Color{}, bool bWire = false );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

/* Line shape struct keeping track of an array of points and color*/
struct Line : public IShape
{
	std::array<glm::vec2, 2> points;
	Color color;

	Line( std::array<glm::vec2, 2> pts, const Color& col = Color{} );
	
	virtual void submit(ShapeRenderer& renderer) const override;
};

/* Creates lua bindings for all available shapes */
struct ShapeBinder
{
	static void CreateLuaBind(sol::state& lua, class Registry& registry);
};

} // jadeite
