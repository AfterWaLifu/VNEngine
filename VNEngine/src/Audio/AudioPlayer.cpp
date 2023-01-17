#include "AudioPlayer.h"
#include "vnepch.h"

#include "Core/Logger.h"

#include <AL/al.h>

namespace VNEngine {

	AudioPlayer::AudioPlayer()
		: m_pDevice(nullptr), m_pContext(nullptr),
		m_MusicSource(0), m_SoundSource(0),
		m_MusicBuffer(0), m_SoundBuffer(0),
		m_AudioList()
	{
		m_pDevice = alcOpenDevice(nullptr);
		if (!m_pDevice) {
			logs.setMeta("Warning");
			logs << "Audio device is not found";
		}

		m_pContext = alcCreateContext(m_pDevice, nullptr);
		if (!m_pContext) {
			if (logs.getMeta().c_str() != "Warning") logs.setMeta("Warning");
			logs << "Audio context creation error";
		}

		if (!alcMakeContextCurrent(m_pContext)) {
			if (logs.getMeta().c_str() != "Warning") logs.setMeta("Warning");
			logs << "Audio context 'make current' error";
		}

		uint32_t tempSources[2] = { 0,0 };
		alGenSources(2, tempSources);
		if (tempSources[0] && tempSources[1]) {
			m_MusicSource = tempSources[0];
			m_SoundSource = tempSources[1];
		}
		else {
			if (logs.getMeta().c_str() != "Warning") logs.setMeta("Warning");
			logs << "Audio sources creation error";
		}

		alSourcef(m_MusicSource, AL_PITCH, 1.0f);
		alSourcef(m_SoundSource, AL_PITCH, 1.0f);
		alSourcef(m_MusicSource, AL_GAIN, 1.0f);
		alSourcef(m_SoundSource, AL_GAIN, 1.0f);
		alSource3f(m_MusicSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(m_SoundSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(m_MusicSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSource3f(m_SoundSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSourcei(m_MusicSource, AL_LOOPING, true);
		alSourcei(m_SoundSource, AL_LOOPING, false);
		alSourcei(m_MusicSource, AL_BUFFER, m_MusicBuffer);
		alSourcei(m_SoundSource, AL_BUFFER, m_SoundBuffer);

		if (logs.getMeta() != "INFO") 	logs.setMeta("INFO");
	}

	AudioPlayer::~AudioPlayer() {
		uint32_t tempSources[2] = { m_MusicSource, m_SoundSource };
		alDeleteSources(2, tempSources);
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);
	}

	void AudioPlayer::PlayMusic(const std::string& trackName) {
		uint32_t buffer = m_AudioList.GetAudio(trackName);

		if (buffer == UINT32_MAX) {
			logs.setMeta("Warning");
			logs << "Audio '" << trackName << "' is not found (make it sure to add it)";
			logs.setMeta("INFO");
			return;
		}

		if (buffer != m_MusicBuffer) {
			m_MusicBuffer = buffer;
			alSourcei(m_MusicSource, AL_BUFFER, (int)m_MusicBuffer);
		}
		alSourcePlay(m_MusicSource);
	}

	void AudioPlayer::PlaySound(const std::string& trackName) {
		uint32_t buffer = m_AudioList.GetAudio(trackName);

		if (buffer == UINT32_MAX) {
			logs.setMeta("Warning");
			logs << "Audio '" << trackName << "' is not found (make it sure to add it)";
			logs.setMeta("INFO");
			return;
		}

		if (buffer != m_SoundBuffer) {
			m_SoundBuffer = buffer;
			alSourcei(m_SoundSource, AL_BUFFER, (int)m_SoundBuffer);
		}
		alSourcePlay(m_SoundSource);
	}

	void AudioPlayer::AddAudio(std::string filename, std::string key) {
		if (m_AudioList.AddAudio(filename, key)) {
			logs << "Audio '" << key << "' has been added";
		}
		else {
			logs.setMeta("Warning");
			logs << "Audio adding '" << key << "' error";
			logs.setMeta("INFO");
		}
	}

	void AudioPlayer::RemoveAudio(std::string key) {
		if (m_AudioList.RemoveAudio(key)) {
			logs << "Audio '" << key << "' has been removed";
		}
		else {
			logs.setMeta("Warning");
			logs << "Audio removing '" << key << "' error";
			logs.setMeta("INFO");
		}
	}

}