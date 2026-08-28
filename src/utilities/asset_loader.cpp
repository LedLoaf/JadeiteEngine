#include "asset_loader.hpp"
#include "rendering/shader.hpp"
#include "rendering/texture.hpp"
#include "rendering/font.hpp"

#include <SDL2/SDL_image.h>
#include <iostream>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>
#include <fstream>

namespace jadeite::utilities
{
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
		
		std::cerr << "GLSL compile failed: " << std::string{ errorLog } << std::endl;
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
		
		std::cerr << "GLSL compile failed: " << std::string{ errorLog } << std::endl;
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
		
		std::cerr << "GLSL Link failed: " << std::string{ errorLog } << std::endl;
		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		
		return nullptr;
	}
	
	if (program == 0)
	{
		std::cerr << "Failed to load shader from memory. Program invalid\n";
		return nullptr;
	}
	
	return std::make_shared<jadeite::Shader>(program);
}

std::shared_ptr<jadeite::Texture> AssetLoader::LoadTexture(const std::string& sFilename, bool bPixelArt )
{
	GLuint texID{ 0 };
	
	SDL_Surface* pSurface = IMG_Load(sFilename.c_str());
	
	if (!pSurface)
	{
		std::cerr << "Failed to create surface from texture file: " << sFilename << "\n";
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

std::shared_ptr<jadeite::Font> AssetLoader::LoadFont(const std::string& sFilename, float fontSize )
{
	int width{ 1024 }, height{ 1024 };
	
	std::ifstream fontStream{ sFilename, std::ios::binary};
	
	if ( fontStream.fail() )
	{
		std::cerr << "Failed to load font from path [" << sFilename << "]\n";
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
	
	int result = stbtt_BakeFontBitmap(
		buffer.data(), 0, fontSize, bitmap.data(), width, height, 32, 96, data.get()
	);
	
	if ( result <= 0 )
	{
		std::cerr << "Font baking failed for: [" << sFilename << "]\n";
		return nullptr;
	}
	
	GLuint id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.data() 
	);
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	return std::make_shared<jadeite::Font>(id, width, height, fontSize, (void*)data.release());
}

Mix_Music* AssetLoader::LoadMusic(const std::string& sFilename)
{
	Mix_Music* pMusic = Mix_LoadMUS(sFilename.c_str());
	if (!pMusic)
	{
		std::string error{ Mix_GetError() };
		std::cerr << "Failed to load music at path [" << sFilename << 
			"] - Error: " << error << "\n";
			
		return nullptr;
	}
	
	return pMusic;
}

Mix_Chunk* AssetLoader::LoadSoundFX(const std::string& sFilename)
{
	Mix_Chunk* pChunk = Mix_LoadWAV(sFilename.c_str());
	if (!pChunk)
	{
		std::string error{ Mix_GetError() };
		std::cerr << "Failed to load soundfx at path [" << sFilename << 
			"] - Error: " << error << "\n";
			
		return nullptr;
	}
	
	return pChunk;
}

} // jadeite::utilities
