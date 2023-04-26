#include "AudioPlayer.h"
#include "vnepch.h"

#include "Core/Logger.h"

#include <AL/al.h>

namespace VNEngine {

	AudioPlayer::AudioPlayer()
		: m_pDevice(nullptr), m_pContext(nullptr),
		m_MusicSource(0), m_SoundSource(0),
		m_MusicBuffer(0), m_SoundBuffer(0),
		m_MusicVolume(1.0f), m_SoundVolume(1.0f),
		m_AudioList(), m_Mute(false),
		m_AudiofilesPath("")
	{
		m_pDevice = alcOpenDevice(nullptr);
		if (!m_pDevice) {
			VN_LOGS_WARNING("Audio device is not found");
			return;
		}

		m_pContext = alcCreateContext(m_pDevice, nullptr);
		if (!m_pContext) {
			VN_LOGS_WARNING("Audio context creation error");
			return;
		}

		if (!alcMakeContextCurrent(m_pContext)) {
			VN_LOGS_WARNING("Audio context 'make current' error");
			return;
		}

		uint32_t tempSources[2] = { 0,0 };
		alGenSources(2, tempSources);
		if (tempSources[0] && tempSources[1]) {
			m_MusicSource = tempSources[0];
			m_SoundSource = tempSources[1];
		}
		else {
			VN_LOGS_WARNING("Audio sources creation error");
			return;
		}

		alSourcef(m_MusicSource, AL_PITCH, 1.0f);
		alSourcef(m_SoundSource, AL_PITCH, 1.0f);
		alSourcef(m_MusicSource, AL_GAIN, m_MusicVolume);
		alSourcef(m_SoundSource, AL_GAIN, m_SoundVolume);
		alSource3f(m_MusicSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(m_SoundSource, AL_POSITION, 0.0f, 0.0f, 0.0f);
		alSource3f(m_MusicSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSource3f(m_SoundSource, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
		alSourcei(m_MusicSource, AL_LOOPING, true);
		alSourcei(m_SoundSource, AL_LOOPING, false);
		alSourcei(m_MusicSource, AL_BUFFER, m_MusicBuffer);
		alSourcei(m_SoundSource, AL_BUFFER, m_SoundBuffer);

		VN_LOGS_INFO("Audio Player initialized successfully");
	}

	AudioPlayer::~AudioPlayer() {
		uint32_t tempSources[2] = { m_MusicSource, m_SoundSource };
		alDeleteSources(2, tempSources);
		alcMakeContextCurrent(nullptr);
		alcDestroyContext(m_pContext);
		alcCloseDevice(m_pDevice);

		VN_LOGS_INFO("Audio Player deinitialized successfully");
	}

	AudioPlayer& AudioPlayer::Instance()
	{
		return s_pInstance ? *s_pInstance : *(s_pInstance = new AudioPlayer());
	}

	void AudioPlayer::PlayMusic(const std::string& trackName) {
		uint32_t buffer = m_AudioList.GetAudio(trackName);

		if (buffer == UINT32_MAX) {
			VN_LOGS_WARNING( "Audiofile '" + trackName + "' is not found (make it sure to add it)");
			return;
		}

		if (buffer != m_MusicBuffer) {
			m_MusicBuffer = buffer;
			alSourcei(m_MusicSource, AL_BUFFER, (int)m_MusicBuffer);
		}
		alSourcePlay(m_MusicSource);
	}

	void AudioPlayer::PauseMusic() {
		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourcePause(m_MusicSource);
		}
		else {
			VN_LOGS_WARNING("Attempted to pause music, but it is not playing");
		}
	}

	void AudioPlayer::ResumeMusic() {
		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PAUSED) {
			alSourcePlay(m_MusicSource);
		}
		else if (state == AL_STOPPED){
			VN_LOGS_WARNING("Attempted to resume music, but it is stopped");
		}
		else {
			VN_LOGS_WARNING("Attempted to resume music, but it is not paused");
		}
	}

	void AudioPlayer::StopMusic() {
		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourceStop(m_MusicSource);
		}
		else {
			VN_LOGS_WARNING("Attempted to stop music, but it is not playing");
		}
	}

	void AudioPlayer::PlaySound(const std::string& trackName) {
		uint32_t buffer = m_AudioList.GetAudio(trackName);

		if (buffer == UINT32_MAX) {
			VN_LOGS_WARNING("Audiofile '" + trackName + "' is not found (make it sure to add it)");
			return;
		}

		if (buffer != m_SoundBuffer) {
			m_SoundBuffer = buffer;
			alSourcei(m_SoundSource, AL_BUFFER, (int)m_SoundBuffer);
		}
		alSourcePlay(m_SoundSource);
	}

	void AudioPlayer::StopSound() {
		int state = AL_STOPPED;
		alGetSourcei(m_SoundSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourceStop(m_SoundSource);
		}
		else {
			VN_LOGS_WARNING("Attempted to stop music, but it is not playing");
		}
	}

	void AudioPlayer::AddAudio(std::string filename, std::string key) {
		if (m_AudioList.AddAudio(m_AudiofilesPath + filename, key)) {
		}
		else {
			VN_LOGS_WARNING("Audio adding '" + m_AudiofilesPath + filename + "' error");
		}
	}

	void AudioPlayer::RemoveAudio(std::string key) {
		if (m_AudioList.RemoveAudio(key)) {
			VN_LOGS_INFO("Audio '" + key + "' has been removed");
		}
		else {
			VN_LOGS_WARNING("Audio removing '" + key + "' error");
		}
	}

	void AudioPlayer::SetMusicVolume(float volume) {
		if (m_Mute) return;
		if (volume != m_MusicVolume) {
			alSourcef(m_MusicSource, AL_GAIN, volume);
			m_MusicVolume = volume;
		}
	}

	void AudioPlayer::SetSoundVolume(float volume) {
		if (m_Mute) return;
		if (volume != m_SoundVolume) {
			alSourcef(m_SoundSource, AL_GAIN, volume);
			m_SoundVolume = volume;
		}
	}

	float AudioPlayer::GetMusicVolume() {
		return m_MusicVolume;
	}

	float AudioPlayer::GetSoundVolume() {
		return m_SoundVolume;
	}

	void AudioPlayer::Mute() {
		m_Mute = true;
		alSourcef(m_MusicSource, AL_GAIN, 0.0f);
		alSourcef(m_SoundSource, AL_GAIN, 0.0f);
	}

	void AudioPlayer::Unmute() {
		m_Mute = false;
		alSourcef(m_MusicSource, AL_GAIN, m_MusicVolume);
		alSourcef(m_SoundSource, AL_GAIN, m_SoundVolume);
	}

	void AudioPlayer::SetAudiofilePath(std::string path) {
		m_AudiofilesPath = path;
	}

	std::string AudioPlayer::GetAudiofilePath() {
		return m_AudiofilesPath;
	}

}