#pragma once 
#include <SDL_mixer.h>
#include <unordered_map>
#include <sol/sol.hpp>
#include <string>


namespace jadeite
{

class Texture;
class Shader;
class Font;

class AssetManager
{
public:
	AssetManager() = default;
	~AssetManager();
	
	bool AddTexture(const std::string& sTextureName, const std::string& sFilename, bool bPixelArt = true);
	std::shared_ptr<Texture> GetTexture(const std::string& sTextureName);
	
	bool AddShaderFromMemory(const std::string& sShaderName, const std::string& sVertData, const std::string& sFragData);
	std::shared_ptr<Shader> GetShader(const std::string& sShaderName);
		
	bool AddFont(const std::string& sFontName, const std::string& sFilename, float fontSize = 32.f);
	std::shared_ptr<Font> GetFont(const std::string& sFontName);
	
	bool AddMusic(const std::string& sMusicName, const std::string& sFilename);
	Mix_Music* GetMusic(const std::string& sMusicName);
	
	bool AddSoundFx(const std::string& sSoundFxName, const std::string& sFilename);
	Mix_Chunk* GetSoundFx(const std::string& sSoundFxName);
	
	bool Clear();
	
	static void CreateLuaBind(sol::state& lua, AssetManager& assetManager);
	
private:
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_mapTextures;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_mapShaders;
	std::unordered_map<std::string, std::shared_ptr<Font>> m_mapFonts;
	std::unordered_map<std::string, Mix_Music*> m_mapMusic;
	std::unordered_map<std::string, Mix_Chunk*> m_mapSoundFx;
};

} // jadeite
