#pragma once 
#include <sol/sol.hpp>

namespace jadeite
{

struct ScriptFuncBinder
{
	static void CreateLuaBind(sol::state& lua);
};

} // jadeite
