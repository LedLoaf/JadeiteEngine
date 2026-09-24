#include "sound_player.hpp"
#include <algorithm>
#include "utilities/asset_manager.hpp"

namespace jadeite
{
	
/* Play the audio */
void SoundPlayer::Play(Mix_Chunk* pChunk, int loops, int channel)
{
	if (!pChunk)
	{
		return;
	}
	
	if (Mix_PlayChannel(channel, pChunk, loops) == -1)
	{
		std::string error = Mix_GetError();
		std::cerr << "Failed to play sound. - " << error << "\n";
	}
}

/* Set the audio volume */
void SoundPlayer::SetVolume(float volume, int channel )
{
	volume = std::clamp(volume, 0.f, 1.f);
	int finalVolume = 128 * volume;
	Mix_Volume(channel, finalVolume);
}

/* Stop the audio playing */
void SoundPlayer::Stop(int channel)
{
	Mix_HaltChannel(channel);
}

/* Check if the audio is playing */
bool SoundPlayer::IsPlaying(int channel)
{
	return Mix_Playing(channel);
}

/* The bindings for lua to access */
void SoundPlayer::CreateLuaBind(sol::state& lua, SoundPlayer& soundPlayer, AssetManager& assetManager)
{
	// SoundPlayer Lua Binding
	lua.new_usertype<SoundPlayer>(
		"SoundPlayer",
		sol::no_constructor,
		"play", 
		sol::overload(
			[&](const std::string& sName)
			{
				auto pSoundFx = assetManager.GetSoundFx( sName );
				if (!pSoundFx)
				{
					std::cerr << "Failed to play soundfx [" << sName 
							  << "] - Does not exist in asset manager.\n";
					return;
				}
				
				soundPlayer.Play(pSoundFx, 0, -1);
			},
			[&](const std::string& sName, int loop, int channel)
			{
				auto pSoundFx = assetManager.GetSoundFx( sName );
				if (!pSoundFx)
				{
					std::cerr << "Failed to play soundfx [" << sName 
							  << "] - Does not exist in asset manager.\n";
					return;
				}
				
				soundPlayer.Play(pSoundFx, loop, channel);
			}
		),
		"stop", [&](int channel) { soundPlayer.Stop(channel); },
		"setVolume", [&](float volume, int channel) { soundPlayer.SetVolume(volume, channel); },
		"isPlaying", [&](int channel) { return soundPlayer.IsPlaying(channel); }
	);
}
} // jadeite::soundPlayer
