#pragma once
#include <SDL_mixer.h>
#include <sol/sol.hpp>

namespace jadeite
{
class MusicPlayer
{
public:
	MusicPlayer();
	MusicPlayer(int frequency, Uint16 format, int channels, int chunksize, int allowedChanges);
	~MusicPlayer();
	
	void Play(Mix_Music* pMusic, int loops = 0);
	void Pause();
	void Resume();
	void Stop();
	void SetVolume(float volume);
	bool IsPlaying() const;
	
	static void CreateLuaBind(sol::state& lua, MusicPlayer& musicPlayer
			, class AssetManager& assetManager);
};
} // jadeite
