#include "vertex.hpp"

namespace jadeite
{
void Vertex::CreateLuaBind(sol::state& lua)
{
	lua.new_usertype<UV>(
		"UV", 
		sol::call_constructor,
		sol::factories(
			[]{ return UV{}; },
			[](float u, float v, float uv_width, float uv_height)
			{
				return UV{ .u = u, .v = v, .uvWidth = uv_width, .uvHeight = uv_height };
			}
		),
		"u", &UV::u,
		"u", &UV::v,
		"uvWidth", &UV::uvWidth,
		"uvHeight", &UV::uvHeight
	);
	
	lua.new_usertype<Color>(
		"Color",
		sol::call_constructor,
		sol::factories(
			[](std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
			{
				return Color{ .r = r, .g = g, .b = b, .a = a };
			}
		),
		"r", &Color::r,
		"g", &Color::g,
		"b", &Color::b,
		"a", &Color::a
	);
	
	lua.set("J2D_WHITE", Color{ 255, 255, 255, 255 } );
	lua.set("J2D_RED", Color{ 255, 0, 0, 255 } );
	lua.set("J2D_GREEN", Color{ 0, 255, 0, 255 } );
	lua.set("J2D_BLUE", Color{ 0, 0, 255, 255 } );
	lua.set("J2D_BLACK", Color{ 0, 0, 0, 255 } );
	lua.set("J2D_YELLOW", Color{ 255, 255, 0, 255 } );
	lua.set("J2D_MAGENTA", Color{ 255, 0, 255, 255 } );
}

} // jadeite