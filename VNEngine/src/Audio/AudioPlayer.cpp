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
		m_AudiofilesPath(""), m_musicPlays(false)
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
		if (trackName.empty() || (trackName == m_CurrentMusic && m_musicPlays)) return;

		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING && trackName == m_CurrentMusic) return;

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
		m_CurrentMusic = trackName;
		m_musicPlays = true;
	}

	void AudioPlayer::PauseMusic() {
		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourcePause(m_MusicSource);
			m_musicPlays = false;
		}
	}

	void AudioPlayer::ResumeMusic() {
		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PAUSED) {
			alSourcePlay(m_MusicSource);
			m_musicPlays = true;
		}
	}

	void AudioPlayer::StopMusic() {
		int state = AL_STOPPED;
		alGetSourcei(m_MusicSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourceStop(m_MusicSource);
			m_musicPlays = false;
		}
	}

	void AudioPlayer::PlaySound(const std::string& trackName) {
		if (trackName.empty()) return;

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
		m_CurrentSound = trackName;
	}

	void AudioPlayer::PauseSound() {
		int state = AL_STOPPED;
		alGetSourcei(m_SoundSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourcePause(m_SoundSource);
		}
	}

	void AudioPlayer::ResumeSoune() {
		int state = AL_STOPPED;
		alGetSourcei(m_SoundSource, AL_SOURCE_STATE, &state);
		if (state == AL_PAUSED) {
			alSourcePlay(m_SoundSource);
		}
	}

	void AudioPlayer::StopSound() {
		int state = AL_STOPPED;
		alGetSourcei(m_SoundSource, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING) {
			alSourceStop(m_SoundSource);
		}
	}

	void AudioPlayer::AddAudio(const std::string& filename, const std::string& key) {
		if (m_AudioList.AddAudio(m_AudiofilesPath + filename, key)) {
		}
		else {
			VN_LOGS_WARNING("Audio adding '" + m_AudiofilesPath + filename + "' error");
		}
	}

	void AudioPlayer::RemoveAudio(const std::string& key) {
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

	bool AudioPlayer::GetIfMute() {
		return m_Mute;
	}

	std::string AudioPlayer::GetCurrentMusic() {
		return m_CurrentMusic;
	}

	std::string AudioPlayer::GetCurrentSound() {
		return m_CurrentSound;
	}

	bool AudioPlayer::GetIfMusicPlays() {
		return m_musicPlays;
	}

	bool AudioPlayer::GetIfSoundPlays()
	{
		int state = AL_STOPPED;
		alGetSourcei(m_SoundSource, AL_SOURCE_STATE, &state);
		return (state == AL_PLAYING);
	}

	void AudioPlayer::PushState() {
		int state = AL_STOPPED;
		alGetSourcei(m_SoundSource, AL_SOURCE_STATE, &state);
		bool splays = state == AL_PLAYING;
		m_States.push_back({m_CurrentMusic,m_CurrentSound,m_musicPlays, splays});
		m_CurrentMusic = "";
		m_CurrentSound = "";
		StopMusic();
		StopSound();
	}

	void AudioPlayer::PopState() {
		if (m_States.empty()) return;
		state s = m_States.back();
		m_States.pop_back();
		m_CurrentMusic = s.mkey;
		m_CurrentSound = s.mkey;
		if (s.mplays) {
			PlayMusic(m_CurrentMusic);
		}
		else {
			m_MusicBuffer = m_AudioList.GetAudio(m_CurrentMusic);
			alSourcei(m_MusicSource, AL_BUFFER, (int)m_MusicBuffer);
		}
		m_SoundBuffer = m_AudioList.GetAudio(m_CurrentSound);
		alSourcei(m_SoundSource, AL_BUFFER, (int)m_SoundBuffer);
	}

	void AudioPlayer::WipeStates() {
		StopMusic();
		StopSound();
		if (m_States.empty()) return;
		m_States.clear();
	}

	void AudioPlayer::SetAudiofilePath(std::string path) {
		m_AudiofilesPath = path;
	}

	std::string AudioPlayer::GetAudiofilePath() {
		return m_AudiofilesPath;
	}

	AudioPlayer::dump AudioPlayer::Dump() {
		if (m_States.empty()) return dump({});
		dump d = {
			m_AudioList.Dump(),
			{m_States.back().mkey,m_States.back().skey,m_States.back().mplays, m_States.back().splays}
		};
		return d;
	}
}