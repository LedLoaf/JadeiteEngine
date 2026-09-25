#pragma once
#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
	/* UV coordinates for textures */
	struct UV
	{
		float u{ 0.f };
		float v{ 0.f };
		float uvWidth{ 0.f };
		float uvHeight{ 0.f };
	};

	/* RGBA color */
	struct Color
	{
		std::uint8_t r{ 255 };
		std::uint8_t g{ 255 };
		std::uint8_t b{ 255 };
		std::uint8_t a{ 255 };
	};

	/* Vertex (position, UV, color) */
	struct Vertex
	{
		glm::vec2 position{ 0.f };
		UV uvs{};
		Color color{ .r = 255, .g = 255, .b = 255, .a = 255 };
		
		static void CreateLuaBind(sol::state& lua);
	};

	/* A shape vertex (position, color) */
	struct ShapeVertex
	{
		glm::vec2 position{ 0.f };
		Color color{ .r = 255, .g = 255, .b = 255, .a = 255 };
	};
} // jadeite
