#pragma once
#include <glm/glm.hpp>
#include <sol/sol.hpp>

namespace jadeite
{
	/* Forward declarations */	
	class AssetManager;
	class Font;
	struct TransformComponent;

	namespace utilities
	{
		/* Various utility functions */
		struct JadeiteUtilities 
		{
			/* Text Utilities */
			static float MeasureText(const std::string& text, Font& font);
			static float RightAlign(const std::string& text, Font& font, const glm::vec2& alignPos);
			static float CenterAlign(const std::string& text, Font& font, const glm::vec2& alignPos);
			
			/* Rotate around the center, not the top-left. Without this rotation would be by the sprites top-left, which doesn't look good. */
			static glm::mat4 RSTModel(const TransformComponent& transform, float width, float height);
			
			/* The bindings for lua to access these utility functions */ 
			static void CreateLuaBind(sol::state& lua, AssetManager& assetManager);
		};
	} // jadeite::utilites
} // jadeite