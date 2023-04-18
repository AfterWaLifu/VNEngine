#pragma once

#include "AL/alc.h"

#include "AudioList.h"

namespace VNEngine {

	/**
	* @brief Audio player class, plays audiofiles
	*/
	class AudioPlayer {
	public:
		/**
		* @brief	Constructors, inits it, needed
		*/
		AudioPlayer();
		/**
		* @brief	Destructor, also needed, but less =)
		*/
		~AudioPlayer();

		/**
		* @brief Plays audio on repeat
		* @param trackName Keycode of added audio
		*/
		void PlayMusic(const std::string& trackName);
		void PauseMusic();	/// Pauses music, may be resumed
		void ResumeMusic();	/// Resumes paused music
		void StopMusic();	/// *holy music stops* and returns to the begining of audio
		/**
		* @brief Plays a sound once
		* @param trackName Keycode of added audio
		*/
		void PlaySound(const std::string& trackName);
		void StopSound();	/// Stops sound

		/**
		* @brief Adds new audio to the app
		* @param filename	The path + name of the file
		* @param key		Keyword to get an access to this audio
		*/
		void AddAudio(std::string filename, std::string key);
		/**
		* @brief Removing audio from the dictionary
		* @param key		Keyword of audio to remove
		*/
		void RemoveAudio(std::string key);

		/**
		* @brief Set a new volume value for music
		* @param volume	Value in range [0.0;1.0]
		*/
		void SetMusicVolume(float volume);
		/**
		* @brief Set a new volume value for sounds
		* @param volume	Value in range [0.0;1.0]
		*/
		void SetSoundVolume(float volume);
		/**
		* @brief Retuns a current volume value for music
		* @return Current volume value for music
		*/
		float GetMusicVolume();
		/**
		* @brief Retuns a current volume value for sounds
		* @return Current volume value for sounds
		*/
		float GetSoundVolume();

		void Mute();	/// Mute all the sounds/musics
		void Unmute();	/// Unmute all the sounds/musics

	private:
		AudioList m_AudioList;	/// Manages audiofiles

		// OPENAL THINGS BELOW, DONT TOUCH THAT
		ALCdevice* m_pDevice;
		ALCcontext* m_pContext;

		uint32_t m_MusicSource;
		uint32_t m_SoundSource;

		uint32_t m_MusicBuffer;
		uint32_t m_SoundBuffer;
		// END OF OPENAL THINGS

		float m_MusicVolume;	/// Music volume
		float m_SoundVolume;	/// Sound volume
		bool m_Mute;			/// Mute state
	};
}