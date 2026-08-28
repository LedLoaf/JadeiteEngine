#include "script_bindings.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include "utilities/timer.hpp"

namespace jadeite
{

using namespace utilities;

void ScriptFuncBinder::CreateLuaBind(sol::state& lua)
{
	lua.set_function("J2D_RunScript", [&](const std::string& sPath)
		{
			try
			{
				auto result = lua.safe_script_file(sPath);
				if (!result.valid())
				{
					sol::error error = result;
					throw error;
				}
			}
			catch( const sol::error& error)
			{
				std::cerr << "Failed to run lua script [: " << sPath << "] - " <<
					error.what() << "\n";
					
				return false;
			}
			
			return true;
		}
	);
	
	lua.set_function("J2D_LoadScriptTable", [&]( const sol::table& scriptTable)
		{
			if (!scriptTable.valid())
			{
				std::cerr << "Failed to load scripts from table. Table is invalid.\n";
				return;
			}
			
			try
			{
				for ( const auto& [_, script] : scriptTable )
				{
					auto result = lua.safe_script_file(script.as<std::string>());
					if (!result.valid())
					{
						sol::error error = result;
						throw error;
					}
				}
			}
			catch (const sol::error& error)
			{
				std::cerr << "Failed to run lua script - " << error.what() << "\n";
				return;
			}
		}
	);
	
	lua.set_function( "J2D_GetTicks", [] { return SDL_GetTicks(); } );
	
	lua.new_usertype<Timer> (
		"Timer",
		sol::call_constructor,
		sol::constructors<Timer()>(),
		"start", &Timer::Start,
		"stop", &Timer::Stop,
		"resume", &Timer::Resume,
		"pause", &Timer::Pause,
		"isRunning", &Timer::IsRunning,
		"isPaused", &Timer::IsPaused,
		"elapsedMs", &Timer::ElapsedMS,
		"elapsedSec", &Timer::ElapsedSec,
		"restart", &Timer::Restart
	);
}



} // jadeite
