#pragma once
#include <sol/sol.hpp>

namespace jadeite
{
	/* Struct of glm binders */
	struct GlmBinder
	{
		static void CreateLuaBind(sol::state& lua);
	};
} // jadeite
