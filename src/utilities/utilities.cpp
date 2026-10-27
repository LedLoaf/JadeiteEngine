#include "utilities.hpp"
#include "utilities/logger.hpp"

#include "asset_manager.hpp"
#include "rendering/font.hpp"
#include "ecs/components.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace jadeite::utilities
{
	/* Returns the total width of `text` in pixels.  */
	float JadeiteUtilities::MeasureText(const std::string& text, Font& font)
	{
		glm::vec2 position{ 0.f };
		for (const auto& character : text)
		{
			font.GetNextCharPos(character, position);
		}
		
		return position.x;
	}

	/* Returns the x to start drawing `text` so its right edge is at `alignPos.x`. */
	float JadeiteUtilities::RightAlign(const std::string& text, Font& font, const glm::vec2& alignPos)
	{
		float textWidth = MeasureText(text, font);
		return alignPos.x - textWidth;
	}

	/* Returns the x to start drawing `text` so its center is at `alignPos.x`. */
	float JadeiteUtilities::CenterAlign(const std::string& text, Font& font, const glm::vec2& alignPos)
	{
		float textWidth = MeasureText(text, font);
		return alignPos.x - (textWidth * 0.5f);
	}
	
	/* Builds a model matrix from a transform + bounding dimensions. Helps the rotation be based on the center origin */
	glm::mat4 JadeiteUtilities::RSTModel(const TransformComponent& transform, float width, float height)
	{
		// The model matrix, where our object scale and rotates around it's own center, which is the behavior we almost ALWAYS want in 2D engines. 
		glm::mat4 model{ 1.f };
		
		// Move quad so its top-left is at origin (local space)
		model = glm::translate(model, glm::vec3{transform.position, 0.f});
		
		// Shift the origin to the center of the sprite (not multiplying by the transform scale will cause the pivot point to still be the top-left corner)
		model = glm::translate(model, glm::vec3{(width * transform.scale.x) * 0.5f, (height * transform.scale.y) * 0.5f, 0.f});
		
		// Apply the rotation around the Z-axis since we are working in 2D
		model = glm::rotate(model, glm::radians(transform.rotation), glm::vec3{0.f, 0.f, 1.f});
		
		// Undo the center shift
		model = glm::translate(model, glm::vec3{(width * transform.scale.x) * -0.5f, (height * transform.scale.y) * -0.5f, 0.f});
		
		// Apply the scale
		model = glm::scale(model, glm::vec3{transform.scale, 1.f});
		
		// Translate back by the negative position to bring everything back to world space
		model = glm::translate(model, glm::vec3{-transform.position, 0.f});
		
		return model;
	}
	

	/* The bindings for lua to access */
	void JadeiteUtilities::CreateLuaBind(sol::state& lua, AssetManager& assetManager)
	{
		// J2D_MeasureText Lua Binding
		lua.set_function(
			"J2D_MeasureText",
			[&](const std::string& sText, const std::string& sFontName)
			{
				if (auto pFont = assetManager.GetFont(sFontName))
				{
					return MeasureText(sText, *pFont);
				}
				
				LogError(Red, std::string("[Utilities] Failed to measure text: Font [") + sFontName + "] does not exist in the asset manager");
			
				return 0.f;
			}
		);
		
		// J2D_RightAlignText Lua Binding
		lua.set_function(
			"J2D_RightAlignText",
			[&](const std::string& sText, const std::string& sFontName, const glm::vec2& alignPos)
			{
				if (auto pFont = assetManager.GetFont(sFontName))
				{
					return RightAlign(sText, *pFont, alignPos);
				}
				
				LogError(Red, std::string("[Utilities] Failed to get right align position: Font [") + sFontName + "] does not exist in the asset manager");
				
				return 0.f;
			}
		);
		
		// J2D_CenterAlignText Lua Binding
		lua.set_function(
			"J2D_CenterAlignText",
			[&](const std::string& sText, const std::string& sFontName, const glm::vec2& alignPos)
			{
				if (auto pFont = assetManager.GetFont(sFontName))
				{
					return CenterAlign(sText, *pFont, alignPos);
				}
				
				LogError(Red, std::string("[Utilities] Failed to get center align position: Font [") + sFontName + "] does not exist in the asset manager");
				
				return 0.f;
			}
		);
	}
} // jadeite::JadeiteUtilities
