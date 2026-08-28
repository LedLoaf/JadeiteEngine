#include "utilities.hpp"
#include "asset_manager.hpp"
#include "rendering/font.hpp"

namespace jadeite::utilities
{
	
float JadeiteUtilities::MeasureText(const std::string& text, Font& font)
{
	glm::vec2 position{ 0.f };
	for (const auto& character : text)
	{
		font.GetNextCharPos(character, position);
	}
	
	return position.x;
}

float JadeiteUtilities::RightAlign(const std::string& text, 
									Font& font, const glm::vec2& alignPos)
{
	float textWidth = MeasureText(text, font);
	return alignPos.x - textWidth;
}

float JadeiteUtilities::CenterAlign(const std::string& text, 
										Font& font, const glm::vec2& alignPos)
{
	float textWidth = MeasureText(text, font);
	return alignPos.x - (textWidth * 0.5f);
}


void JadeiteUtilities::CreateLuaBind(sol::state& lua, AssetManager& assetManager)
{
	lua.set_function(
		"J2D_MeasureText",
		[&](const std::string& sText, const std::string& sFontName)
		{
			if (auto pFont = assetManager.GetFont(sFontName))
			{
				return MeasureText(sText, *pFont);
			}
			
			std::cerr << "Failed to measure text. Font [" << sFontName 
					<< "] does not exist in asset manager.\n";
			return 0.f;
		}
	);
	
	lua.set_function(
		"J2D_RightAlignText",
		[&](const std::string& sText, const std::string& sFontName, const glm::vec2& alignPos)
		{
			if (auto pFont = assetManager.GetFont(sFontName))
			{
				return RightAlign(sText, *pFont, alignPos);
			}
			
			std::cerr << "Failed to get right align position. Font [" << sFontName 
					<< "] does not exist in asset manager.\n";
					
			return 0.f;
		}
	);
	
	lua.set_function(
		"J2D_CenterAlignText",
		[&](const std::string& sText, const std::string& sFontName, const glm::vec2& alignPos)
		{
			if (auto pFont = assetManager.GetFont(sFontName))
			{
				return CenterAlign(sText, *pFont, alignPos);
			}
			
			std::cerr << "Failed to get center align position. Font [" << sFontName 
					<< "] does not exist in asset manager.\n";
					
			return 0.f;
		}
	);
}

} // jadeite::utilities
