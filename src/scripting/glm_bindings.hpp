#pragma once
#include <sol/sol.hpp>

namespace jadeite
{
struct GlmBinder
{
	static void CreateLuaBind(sol::state& lua);
};
} // jadeite
