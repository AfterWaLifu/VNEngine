#pragma once

#include "AL/alc.h"

#include "AudioList.h"

namespace VNEngine {

	class AudioPlayer {
	public:
		AudioPlayer();
		~AudioPlayer();

		void PlayMusic(const std::string& trackName);
		void PlaySound(const std::string& trackName);

		void AddAudio(std::string filename, std::string key);
		void RemoveAudio(std::string key);

	private:
		AudioList m_AudioList;

		ALCdevice* m_pDevice;
		ALCcontext* m_pContext;

		uint32_t m_MusicSource;
		uint32_t m_SoundSource;

		uint32_t m_MusicBuffer;
		uint32_t m_SoundBuffer;
	};
}