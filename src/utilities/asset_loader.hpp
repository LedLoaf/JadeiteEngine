#pragma once
#include <string>
#include <SDL_mixer.h>

namespace jadeite
{
class Shader;
class Texture;
class Font;

namespace utilities
{

class AssetLoader
{
public:
	AssetLoader() = delete;
	
	static std::shared_ptr<jadeite::Shader> LoadShaderFromMemory(const char* vertexShader, const char* fragmentShader);
	static std::shared_ptr<jadeite::Texture> LoadTexture(const std::string& sFilename, bool bPixelArt);
	static std::shared_ptr<jadeite::Font> LoadFont(const std::string& sFilename, float fontSize = 32.f);
	static Mix_Music* LoadMusic(const std::string& sFilename);
	static Mix_Chunk* LoadSoundFX(const std::string& sFilename);
	
};

} // utilities	
} // jadeite
