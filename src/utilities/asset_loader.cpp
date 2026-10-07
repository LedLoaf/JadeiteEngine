#include "asset_loader.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/font.hpp"
#include "utilities/logger.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <fstream>
// TODO: Add the ability to load from file

namespace jadeite::utilities
{		
	/* Loads the vertex and fragment shader from memory */
	std::shared_ptr<jadeite::Shader> AssetLoader::LoadShaderFromMemory(const char* vertexShader, const char* fragmentShader)
	{
		const GLuint program = glCreateProgram();
		
		// Create the vertex shader
		const GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vertexShader, nullptr);
		glCompileShader(vertShader);
		
		GLint status;
		
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &status);
		
		if (status != GL_TRUE)
		{
			GLint maxLength;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);
			
			std::string errorLog(maxLength, ' ');
			glGetShaderInfoLog(vertShader, maxLength, &maxLength, errorLog.data());
			
			LogError(Red, std::string("[AssetLoader] GLSL Vertex Compile Failed: ") + errorLog);

			glDeleteShader(vertShader);
			return nullptr;
		}
		
		// Create the fragment shader
		const GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fragmentShader, nullptr);
		glCompileShader(fragShader);
		
		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
		
		if (status != GL_TRUE)
		{
			GLint maxLength;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);
			
			std::string errorLog(maxLength, ' ');
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, errorLog.data());
			
			LogError(Red, std::string("[AssetLoader] GLSL Fragment Compile Failed: ") + errorLog);
			
			glDeleteShader(fragShader);
			return nullptr;
		}
		
		if (vertShader == 0 || fragShader == 0)
			return nullptr;
		
		// Attach the shaders to the program
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		
		// Link the two shaders
		glLinkProgram(program);
		
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		
		if (status != GL_TRUE)
		{
			GLint maxLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			
			std::string errorLog(maxLength, ' ');
			glGetProgramInfoLog(program, maxLength, &maxLength, errorLog.data());
			
			LogError(Red, std::string("[AssetLoader] GLSL Program Link Failed: ") + errorLog);
			
			glDeleteShader(vertShader);
			glDeleteShader(fragShader);
			
			return nullptr;
		}
		
		if (program == 0)
		{
			LogError(Red, "[AssetLoader] Failed to load shader from memory; Program invalid...");
			return nullptr;
		}

		return std::make_shared<jadeite::Shader>(program);
	}

	/* Load a texture and set if it's pixel art or not */
	std::shared_ptr<jadeite::Texture> AssetLoader::LoadTexture(const std::string& sFilename, bool bPixelArt )
	{
		GLuint texID{ 0 };
		
		SDL_Surface* pSurface = IMG_Load(sFilename.c_str());
		
		if (!pSurface)
		{
			LogError(Red, std::string("[AssetLoader] Failed to create surface from texture file: ") + sFilename);
			return nullptr;
		}
		
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		
		int format{ GL_RGBA };
		
		SDL_Surface* pFormattedSurface{ nullptr };
		if (pSurface->format->BytesPerPixel == 3)
		{
			pFormattedSurface = SDL_ConvertSurfaceFormat(pSurface, SDL_PIXELFORMAT_RGB24, 0);
			format = GL_RGB;
		}
		else
		{
			pFormattedSurface = SDL_ConvertSurfaceFormat(pSurface, SDL_PIXELFORMAT_RGBA32, 0);
			format = GL_RGBA;
		}
		
		int width{ pFormattedSurface->w };
		int height{ pFormattedSurface->h };
		
		glTexImage2D(
			GL_TEXTURE_2D, 
			0, 
			format,
			width,
			height,
			0,
			format,
			GL_UNSIGNED_BYTE,
			pFormattedSurface->pixels
		);
		
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_S, 
			GL_CLAMP_TO_EDGE
		);
		
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_WRAP_T, 
			GL_CLAMP_TO_EDGE
		);
		
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MIN_FILTER,
			bPixelArt ? GL_NEAREST : GL_LINEAR
		);
		
		glTexParameteri(
			GL_TEXTURE_2D,
			GL_TEXTURE_MAG_FILTER,
			bPixelArt ? GL_NEAREST : GL_LINEAR
		);
		
		SDL_FreeSurface(pFormattedSurface);
		SDL_FreeSurface(pSurface);
		
		return std::make_shared<jadeite::Texture>(texID, width, height, sFilename);
	}

	/* Load a font file */
	std::shared_ptr<jadeite::Font> AssetLoader::LoadFont(const std::string& sFilename, float fontSize )
	{
		int width{ 1024 }, height{ 1024 };
		
		std::ifstream fontStream{ sFilename, std::ios::binary};
		
		if ( fontStream.fail() )
		{
			LogError(Red, std::string("[AssetLoader] Failed to load font from path: ") + sFilename);
			return nullptr;
		}
		
		fontStream.seekg(0, fontStream.end);
		int64_t length = fontStream.tellg();
		
		fontStream.seekg(0, fontStream.beg);
		
		std::vector<unsigned char> buffer;
		buffer.resize( length );
		
		std::vector<unsigned char> bitmap;
		bitmap.resize( width * height );
		
		fontStream.read( (char*)(&buffer[0]), length );
		
		auto data = std::make_unique<stbtt_bakedchar[]>(96);
		
		
		// Create the font data with the size
		const float bakedSize = 64.0f;  		// THIS WILL ALWAYS CREATE THE TEXT AT SIZE 64 WHERE IT WILL BE DOWNSCALED
		int result = stbtt_BakeFontBitmap(buffer.data(), 0, bakedSize, bitmap.data(), width, height, 32, 96, data.get());
		
		if ( result <= 0 )
		{
			LogError(Red, std::string("[AssetLoader] Failed Font Baking: ") + sFilename);
			return nullptr;
		}
		
		GLuint id;
		glGenTextures(1, &id);
		
		glBindTexture(GL_TEXTURE_2D, id);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA  , width, height, 0, GL_ALPHA  , GL_UNSIGNED_BYTE, bitmap.data() );
		glGenerateMipmap(GL_TEXTURE_2D);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		return std::make_shared<jadeite::Font>(id, width, height, bakedSize, fontSize, (void*)data.release());
	}

	/* Load a Music file */
	Mix_Music* AssetLoader::LoadMusic(const std::string& sFilename)
	{
		Mix_Music* pMusic = Mix_LoadMUS(sFilename.c_str());
		if (!pMusic)
		{
			std::string error{ Mix_GetError() };
			LogError(Red, std::string("[AssetLoader] Failed to load music at path [") + sFilename + "] - Error: " + error); 
			
			return nullptr;
		}
		
		return pMusic;
	}

	/* Load a sound effect file */
	Mix_Chunk* AssetLoader::LoadSoundFX(const std::string& sFilename)
	{
		Mix_Chunk* pChunk = Mix_LoadWAV(sFilename.c_str());
		if (!pChunk)
		{
			std::string error{ Mix_GetError() };
			LogError(Red, std::string("[AssetLoader] Failed to load sound at path [") + sFilename + "] - Error: " + error);
			
			return nullptr;
		}
	
		return pChunk;
	}
} // jadeite::AssetLoader
