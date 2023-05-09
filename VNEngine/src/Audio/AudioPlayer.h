#pragma once

#include "AL/alc.h"

#include "AudioList.h"

#include <vector>

namespace VNEngine {

	/**
	* @brief Audio player class, plays audiofiles
	*/
	class AudioPlayer {
	public:
		static AudioPlayer& Instance();

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
		void AddAudio(const std::string& filename, const std::string& key);
		/**
		* @brief Removing audio from the dictionary
		* @param key		Keyword of audio to remove
		*/
		void RemoveAudio(const std::string& key);

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
		bool GetIfMute();	///	Returns true if muted

		std::string GetCurrentMusic();	/// Returns a key of current music
		std::string GetCurrentSound();	/// Returns a key of current sound
		bool GetIfPlays();	// Returns true if music plays

		void PushState();
		void PopState();
		void WipeStates();

		void SetAudiofilePath(std::string path);
		std::string GetAudiofilePath();

	private:
		static inline AudioPlayer* s_pInstance;
		
		std::string m_AudiofilesPath;

		/**
		* @brief	Constructors, inits it, needed
		*/
		AudioPlayer();
		/**
		* @brief	Destructor, also needed, but less =)
		*/
		~AudioPlayer();

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

		std::string m_CurrentMusic;		/// current music key
		std::string m_CurrentSound;		/// current sound key
		bool m_plays;	// music is currently playing

		struct state {
			std::string mkey;
			std::string skey;
			bool plays;
		};
		std::vector<state> m_States;
	};

#define AP_INSTANCE AudioPlayer::Instance()
}