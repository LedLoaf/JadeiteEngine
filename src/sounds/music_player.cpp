#include "music_player.hpp"
#include <iostream>
#include <algorithm>
#include "utilities/asset_manager.hpp"

namespace jadeite
{	
	/* Music audio flags */
	constexpr int DEFAULT_FREQUENCY = 44100; // Hz - CD Quality
	constexpr int DEFAULT_CHANNELS = 2;
	constexpr int DEFAULT_CHUNKSIZE = 1152;
	constexpr int DEFAULT_MIXER_FLAGS = SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE;
	constexpr Uint16 DEFAULT_FORMAT = MIX_DEFAULT_FORMAT;

	/* Default constructor */
	MusicPlayer::MusicPlayer()
		: MusicPlayer(DEFAULT_FREQUENCY, DEFAULT_FORMAT, DEFAULT_CHANNELS, DEFAULT_CHUNKSIZE, DEFAULT_MIXER_FLAGS)
	{
	}

	/* MusicPlayer constructor */
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

	/* MusicPlayer destructor */
	MusicPlayer::~MusicPlayer()
	{
		Mix_HaltMusic();
		Mix_CloseAudio();
		Mix_Quit();
	}

	/* Plays the music */
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

	/* Pauses the music until resumed */
	void MusicPlayer::Pause()
	{
		Mix_PauseMusic();
	}

	/* Resumes the music if paused */
	void MusicPlayer::Resume()
	{
		Mix_ResumeMusic();
	}

	/* Stops the music from playing */
	void MusicPlayer::Stop()
	{
		Mix_HaltMusic();
	}

	/* Sets the music volume */
	void MusicPlayer::SetVolume(float volume)
	{
		volume = std::clamp(volume, 0.f, 1.f);
		int finalVolume = 128 * volume;
		Mix_VolumeMusic(finalVolume);
	}

	/* Check if music is playing */
	bool MusicPlayer::IsPlaying() const
	{
		return Mix_PlayingMusic();
	}

	/* The bindings for lua to access */
	void MusicPlayer::CreateLuaBind(sol::state& lua, MusicPlayer& musicPlayer, AssetManager& assetManager)
	{
		// MusicPlayer Lua Binding
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
						std::cerr 	<< "Failed to play song [" << sName 
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
						std::cerr 	<< "Failed to play song [" << sName 
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
} // jadeite::MusicPlayer
