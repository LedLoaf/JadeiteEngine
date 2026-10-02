#include "asset_manager.hpp"
#include "asset_loader.hpp"

#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/font.hpp"

#include "utilities/logger.hpp"
#include <iostream>
#include <iomanip>

namespace jadeite
{
	/* Destructor */
	AssetManager::~AssetManager()
	{
		Clear();
	}

	/* Adds a texture to the asset manager */
	bool AssetManager::AddTexture(const std::string& sTextureName, const std::string& sFilename, bool bPixelArt)
	{
		if (m_mapTextures.contains(sTextureName))
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add texture [") + sTextureName + "] - Already Exists...");   
			return false;
		}
		
		auto pTexture = utilities::AssetLoader::LoadTexture(sFilename, bPixelArt );
		if (!pTexture)
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to load texture... [") + sTextureName + "]");   
			return false;
		}
		
		Print(Green, "Adding Texture: ", "[" + sTextureName + "]", 20, 20);					// debug text
		
		
		auto [iter, bInserted] = m_mapTextures.emplace(sTextureName, std::move(pTexture));
		return bInserted;
	}

	/* Retrieve the texture from the texture map */
	std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& sTextureName)
	{
		auto textureItr = m_mapTextures.find(sTextureName);
		if (textureItr == m_mapTextures.end())
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to get texture... [") + sTextureName + "] - Does Not Exist");   
			return nullptr;
		}
		
		return textureItr->second;
	}

	/* Adds a shader from memory to the map shaders */
	bool AssetManager::AddShaderFromMemory(const std::string& sShaderName, const std::string& sVertData, const std::string& sFragData)
	{
		if (m_mapShaders.contains(sShaderName))
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add shader... [") + sShaderName + "] - Already Exists"); 
			return false;
		}
		
		auto pShader = utilities::AssetLoader::LoadShaderFromMemory(sVertData.c_str(), sFragData.c_str() );
		if (!pShader)
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add shader... [") + sShaderName + "]"); 
			return false;
		}
		
		Print(Green, "Adding Shader: ", "[" + sShaderName + "]");					// debug text
		
		auto [iter, bInserted] = m_mapShaders.emplace(sShaderName, std::move(pShader));
		return bInserted;
	}

	/* Retrieves a shader by filename */
	std::shared_ptr<Shader> AssetManager::GetShader(const std::string& sShaderName)
	{
		auto shaderItr = m_mapShaders.find(sShaderName);
		if (shaderItr == m_mapShaders.end())
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to get shader... [") + sShaderName + "] - Does Not Exist"); 
			return nullptr;
		}
		
		return shaderItr->second;
	}
		
	/* Adds a font to the asset manager */
	bool AssetManager::AddFont(const std::string& sFontName, const std::string& sFilename, float fontSize)\
	{
		if (m_mapFonts.contains(sFontName))
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add Font... [") + sFontName + "] [" + sFilename + "] - Already Exists"); 
			return false;
		}
		
		auto pFont = utilities::AssetLoader::LoadFont( sFilename, fontSize );
		if (!pFont)
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to load Font... [") + sFontName + "] [" + sFilename + "]");  
			return false;
		}
		
		Print(Green, "Adding Font: ", " [" + sFontName + "]");					// debug text   

		auto [iter, bInserted] = m_mapFonts.emplace(sFontName, std::move(pFont));
		return bInserted;
	}

	/* Retrieve a font from the asset manager */
	std::shared_ptr<Font> AssetManager::GetFont(const std::string& sFontName)
	{
		auto fontItr = m_mapFonts.find(sFontName);
		if (fontItr == m_mapFonts.end())
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add Font... [") + sFontName + "] - Does Not Exists"); 
			return nullptr;
		}
		
		return fontItr->second;
	}

	/* Adds music to the asset manager */
	bool AssetManager::AddMusic(const std::string& sMusicName, const std::string& sFilename)
	{
		if (m_mapMusic.contains(sMusicName))
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add Music... [") + sMusicName + "] [" + sFilename + "] - Already Exists"); 
			return false;
		}
		
		auto* pMusic = utilities::AssetLoader::LoadMusic( sFilename );
		if (!pMusic)
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to load Music... [") + sMusicName + "]"); 
			return false;
		}
		
		Print(Green, "Adding Music: ", " [" + sMusicName + "]");   

		auto [iter, bInserted] = m_mapMusic.emplace(sMusicName, pMusic);
		return bInserted;
	}

	/* Retrieve music from the music map */
	Mix_Music* AssetManager::GetMusic(const std::string& sMusicName)
	{
		auto musicItr = m_mapMusic.find(sMusicName);
		if (musicItr == m_mapMusic.end())
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to get Music... [") + sMusicName + "] - Does Not Exist"); 
			return nullptr;
		}
		
		return musicItr->second;
	}

	/* Add sound effects to the asset manager */
	bool AssetManager::AddSoundFx(const std::string& sSoundFxName, const std::string& sFilename)
	{
		if (m_mapSoundFx.contains(sSoundFxName))
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to add Sound... [") + sSoundFxName + "] [" + sFilename + "] - Already Exists"); 
			return false;
		}
		
		auto* pSoundfx = utilities::AssetLoader::LoadSoundFX( sFilename );
		if (!pSoundfx)
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to load Sound... [") + sSoundFxName + "]"); 
			return false;
		}
		
		Print(Green, "Adding Sound: ", " [" + sSoundFxName + "]");

		auto [iter, bInserted] = m_mapSoundFx.emplace(sSoundFxName, pSoundfx);
		return bInserted;
	}

	/* Retrieve sound effects from the soundFx map */
	Mix_Chunk* AssetManager::GetSoundFx(const std::string& sSoundFxName)
	{
		auto soundfxItr = m_mapSoundFx.find(sSoundFxName);
		if (soundfxItr == m_mapSoundFx.end())
		{
			LogError(BrightRed, std::string("[AssetManager] Failed to get Sound... [") + sSoundFxName + "] - Does Not Exist"); 
			return nullptr;
		}
		
		return soundfxItr->second;
	}

	/* Clears the textures, fonts, and shaders from the asset manager */
	bool AssetManager::Clear()
	{
		m_mapTextures.clear();
		m_mapFonts.clear();
		m_mapShaders.clear();
		
		for (auto& [_, pMusic] : m_mapMusic)
		{
			Mix_FreeMusic( pMusic );
		}
		
		for (auto& [_, pSoundfx ] : m_mapSoundFx)
		{
			Mix_FreeChunk( pSoundfx );
		}
		
		return true;
	}

	/* The bindings for lua to access */
	void AssetManager::CreateLuaBind(sol::state& lua, AssetManager& assetManager)
	{
		// AssetManager Lua Binding
		lua.new_usertype<AssetManager>(
			"AssetManager",
			sol::no_constructor,
			"addTexture", [&](const std::string& sName, const std::string& sPath, bool bPixelArt)
			{
				return assetManager.AddTexture(sName, sPath, bPixelArt);
			},
			"getTexture", [&](const std::string& sName)
			{
				return assetManager.GetTexture(sName);
			},
			"addFont", [&](const std::string& sName, const std::string& sPath, float fontSize)
			{
				return assetManager.AddFont(sName, sPath, fontSize);
			},
			"addMusic", [&](const std::string& sName, const std::string& sPath )
			{
				return assetManager.AddMusic(sName, sPath);
			},
			"addSoundfx", [&](const std::string& sName, const std::string& sPath )
			{
				return assetManager.AddSoundFx(sName, sPath);
			}
		);
	}
} // jadeite::AssetManager
