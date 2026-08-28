#include "music_player.hpp"
#include <iostream>
#include <algorithm>
#include "utilities/asset_manager.hpp"

namespace jadeite
{
	
constexpr int DEFAULT_FREQUENCY = 44100; // Hz - CD Quality
constexpr int DEFAULT_CHANNELS = 2;
constexpr int DEFAULT_CHUNKSIZE = 1152;
constexpr int DEFAULT_MIXER_FLAGS = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE;
constexpr Uint16 DEFAULT_FORMAT = MIX_DEFAULT_FORMAT;

MusicPlayer::MusicPlayer()
	: MusicPlayer(DEFAULT_FREQUENCY, DEFAULT_FORMAT, DEFAULT_CHANNELS, DEFAULT_CHUNKSIZE, DEFAULT_MIXER_FLAGS)
{
}

MusicPlayer::MusicPlayer(int frequency, Uint16 format, int channels, int chunksize, int allowedChanges)
{
	if (Mix_OpenAudioDevice(frequency, format, channels, chunksize, NULL, allowedChanges) == -1)
	{
		std::string error = Mix_GetError();
		std::cerr << "Unable to open the SDL Music Mixer - " << error << "\n";
		return;
	}
	
	// TODO: add different inits as needed
	Mix_Init(MIX_INIT_OGG);
}

MusicPlayer::~MusicPlayer()
{
	Mix_HaltMusic();
	Mix_CloseAudio();
	Mix_Quit();
}

void MusicPlayer::Play(Mix_Music* pMusic, int loops)
{
	if (!pMusic)
	{
		return;
	}
	
	if (Mix_PlayMusic(pMusic, loops) == -1)
	{
		// TODO: add error
	}
}

void MusicPlayer::Pause()
{
	Mix_PauseMusic();
}

void MusicPlayer::Resume()
{
	Mix_ResumeMusic();
}

void MusicPlayer::Stop()
{
	Mix_HaltMusic();
}

void MusicPlayer::SetVolume(float volume)
{
	volume = std::clamp(volume, 0.f, 1.f);
	int finalVolume = 128 * volume;
	Mix_VolumeMusic(finalVolume);
}

bool MusicPlayer::IsPlaying() const
{
	return Mix_PlayingMusic();
}

void MusicPlayer::CreateLuaBind(sol::state& lua, MusicPlayer& musicPlayer, AssetManager& assetManager)
{
	lua.new_usertype<MusicPlayer>(
		"MusicPlayer",
		sol::no_constructor,
		"play",
		sol::overload(
			[&](const std::string& sName, int loops)
			{
				auto pMusic = assetManager.GetMusic(sName);
				if (!pMusic)
				{
					std::cerr << "Failed to play song [" << sName 
						<< "] - Does not exists in asset manager.\n";
					return;
				}
				
				musicPlayer.Play(pMusic, loops);
			},
			[&](const std::string& sName)
			{
				auto pMusic = assetManager.GetMusic(sName);
				if (!pMusic)
				{
					std::cerr << "Failed to play song [" << sName 
						<< "] - Does not exists in asset manager.\n";
					return;
				}
				
				musicPlayer.Play(pMusic, -1);
			}
		),
		"stop", [&]{ return musicPlayer.Stop(); },
		"pause", [&]{ return musicPlayer.Pause(); },
		"resume", [&]{ return musicPlayer.Resume(); },
		"setVolume", [&](float volume) { musicPlayer.SetVolume(volume); },
		"isPlaying", [&]{ return musicPlayer.IsPlaying(); } 
	);
}

} // jadeite
