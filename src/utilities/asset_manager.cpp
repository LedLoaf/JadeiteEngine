#include "asset_manager.hpp"
#include "asset_loader.hpp"

#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/font.hpp"

#include <iostream>

namespace jadeite
{
AssetManager::~AssetManager()
{
	Clear();
}

bool AssetManager::AddTexture(const std::string& sTextureName, const std::string& sFilename, bool bPixelArt)
{
	if (m_mapTextures.contains(sTextureName))
	{
		std::cerr << "Failed to add texture [" << sTextureName << "] - Already Exists.\n";
		return false;
	}
	
	auto pTexture = utilities::AssetLoader::LoadTexture(sFilename, bPixelArt );
	if (!pTexture)
	{
		std::cerr << "Failed to load texture [" << sTextureName << "].\n";
		return false;
	}
	
	auto [iter, bInserted] = m_mapTextures.emplace(sTextureName, std::move(pTexture));
	return bInserted;
}

std::shared_ptr<Texture> AssetManager::GetTexture(const std::string& sTextureName)
{
	auto textureItr = m_mapTextures.find(sTextureName);
	if (textureItr == m_mapTextures.end())
	{
		std::cerr << "Failed to get texture [" << sTextureName << "] - Does not exist.\n";
		return nullptr;
	}
	
	return textureItr->second;
}

bool AssetManager::AddShaderFromMemory(const std::string& sShaderName, const std::string& sVertData, const std::string& sFragData)
{
	if (m_mapShaders.contains(sShaderName))
	{
		std::cerr << "Failed to add shader [" << sShaderName << "] - Already Exists.\n";
		return false;
	}
	
	auto pShader = utilities::AssetLoader::LoadShaderFromMemory(sVertData.c_str(), sFragData.c_str() );
	if (!pShader)
	{
		std::cerr << "Failed to load shader [" << sShaderName << "].\n";
		return false;
	}
	
	auto [iter, bInserted] = m_mapShaders.emplace(sShaderName, std::move(pShader));
	return bInserted;
}

std::shared_ptr<Shader> AssetManager::GetShader(const std::string& sShaderName)
{
	auto shaderItr = m_mapShaders.find(sShaderName);
	if (shaderItr == m_mapShaders.end())
	{
		std::cerr << "Failed to get shader [" << sShaderName << "] - Does not exist.\n";
		return nullptr;
	}
	
	return shaderItr->second;
}
	
bool AssetManager::AddFont(const std::string& sFontName, const std::string& sFilename, float fontSize)\
{
	if (m_mapFonts.contains(sFontName))
	{
		std::cerr << "Failed to add Font [" << sFontName << "] - Already Exists.\n";
		return false;
	}
	
	auto pFont = utilities::AssetLoader::LoadFont( sFilename, fontSize );
	if (!pFont)
	{
		std::cerr << "Failed to load font [" << sFontName << "].\n";
		return false;
	}
	
	auto [iter, bInserted] = m_mapFonts.emplace(sFontName, std::move(pFont));
	return bInserted;
}

std::shared_ptr<Font> AssetManager::GetFont(const std::string& sFontName)
{
	auto fontItr = m_mapFonts.find(sFontName);
	if (fontItr == m_mapFonts.end())
	{
		std::cerr << "Failed to get font [" << sFontName << "] - Does not exist.\n";
		return nullptr;
	}
	
	return fontItr->second;
}

bool AssetManager::AddMusic(const std::string& sMusicName, const std::string& sFilename)
{
	if (m_mapMusic.contains(sMusicName))
	{
		std::cerr << "Failed to add music [" << sMusicName << "] - Already Exists.\n";
		return false;
	}
	
	auto* pMusic = utilities::AssetLoader::LoadMusic( sFilename );
	if (!pMusic)
	{
		std::cerr << "Failed to load music [" << sMusicName << "].\n";
		return false;
	}
	
	auto [iter, bInserted] = m_mapMusic.emplace(sMusicName, pMusic);
	return bInserted;
}

Mix_Music* AssetManager::GetMusic(const std::string& sMusicName)
{
	auto musicItr = m_mapMusic.find(sMusicName);
	if (musicItr == m_mapMusic.end())
	{
		std::cerr << "Failed to get music [" << sMusicName << "] - Does not exist.\n";
		return nullptr;
	}
	
	return musicItr->second;
}

bool AssetManager::AddSoundFx(const std::string& sSoundFxName, const std::string& sFilename)
{
	if (m_mapSoundFx.contains(sSoundFxName))
	{
		std::cerr << "Failed to add soundfx [" << sSoundFxName << "] - Already Exists.\n";
		return false;
	}
	
	auto* pSoundfx = utilities::AssetLoader::LoadSoundFX( sFilename );
	if (!pSoundfx)
	{
		std::cerr << "Failed to load soundfx [" << sSoundFxName << "].\n";
		return false;
	}
	
	auto [iter, bInserted] = m_mapSoundFx.emplace(sSoundFxName, pSoundfx);
	return bInserted;
}

Mix_Chunk* AssetManager::GetSoundFx(const std::string& sSoundFxName)
{
	auto soundfxItr = m_mapSoundFx.find(sSoundFxName);
	if (soundfxItr == m_mapSoundFx.end())
	{
		std::cerr << "Failed to get soundfx [" << sSoundFxName << "] - Does not exist.\n";
		return nullptr;
	}
	
	return soundfxItr->second;
}

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

void AssetManager::CreateLuaBind(sol::state& lua, AssetManager& assetManager)
{
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

} // jadeite
