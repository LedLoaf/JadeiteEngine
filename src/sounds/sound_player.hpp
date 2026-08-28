#pragma once 
#include <SDL_mixer.h>
#include <sol/sol.hpp>

namespace jadeite
{
class SoundPlayer
{
public:
	SoundPlayer() = default;
	~SoundPlayer() = default;
	
	void Play(Mix_Chunk* pChunk, int loops = 0, int channel = -1);
	void SetVolume(float volume, int channel = -1);
	void Stop(int channel);
	bool IsPlaying(int channel);
	
	static void CreateLuaBind(sol::state& lua, SoundPlayer& soundPlayer, 
				class AssetManager& assetManager);
};
} // jadeite
