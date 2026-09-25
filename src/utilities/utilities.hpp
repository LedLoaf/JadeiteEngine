#pragma once
#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
	/* Forward declarations */	
	class Font;
	class AssetManager;

	namespace utilities
	{
		/* Various utility functions */
		struct JadeiteUtilities 
		{
			// Text Utilities
			static float MeasureText(const std::string& text, Font& font);
			static float RightAlign(const std::string& text, Font& font, const glm::vec2& alignPos);
			static float CenterAlign(const std::string& text, Font& font, const glm::vec2& alignPos);
			
			
			static void CreateLuaBind(sol::state& lua, AssetManager& assetManager);
		};
	} // utilites
} // jadeite