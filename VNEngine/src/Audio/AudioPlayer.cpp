#include <VNEngine/Audio/AudioPlayer.hpp>

#include <VNEngine/Core/Logger.hpp>

#include <AL/al.h>

namespace VNEngine {

AudioPlayer::AudioPlayer()
    : audiofilesPath_(""), audioList_(), device_(nullptr), context_(nullptr), musicSource_(0), soundSource_(0),
      musicBuffer_(0), soundBuffer_(0), musicVolume_(1.0f), soundVolume_(1.0f), isMute_(false) {

    device_ = alcOpenDevice(nullptr);
    if (!device_) {
        VN_LOGS_ERROR("Audio device is not found");
        return;
    }

    context_ = alcCreateContext(device_, nullptr);
    if (!context_) {
        VN_LOGS_ERROR("Audio context creation failed");
        return;
    }

    if (!alcMakeContextCurrent(context_)) {
        VN_LOGS_ERROR("Audio context 'make current' failed");
        return;
    }

    std::uint32_t tempSources[2] = {0, 0};
    alGenSources(2, tempSources);
    if (tempSources[0] && tempSources[1]) {
        musicSource_ = tempSources[0];
        soundSource_ = tempSources[1];
    } else {
        VN_LOGS_ERROR("Audio sources creation failed");
        return;
    }

    auto configureSource = [](std::uint32_t source, float volume, std::uint32_t buffer, bool isLooping) {
        alSourcef(source, AL_PITCH, 1.0f);
        alSourcef(source, AL_GAIN, volume);
        alSource3f(source, AL_POSITION, 0.0f, 0.0f, 0.0f);
        alSource3f(source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
        alSourcei(source, AL_LOOPING, isLooping);
        alSourcei(source, AL_BUFFER, buffer);
    };

    configureSource(musicSource_, musicVolume_, musicBuffer_, true);
    configureSource(soundSource_, soundVolume_, soundBuffer_, false);

    VN_LOGS_INFO("Audio Player initialized successfully");
}

AudioPlayer::~AudioPlayer() {
    uint32_t tempSources[2] = {musicSource_, soundSource_};
    alDeleteSources(2, tempSources);
    alcMakeContextCurrent(nullptr);
    alcDestroyContext(context_);
    alcCloseDevice(device_);

    VN_LOGS_INFO("Audio Player deinitialized successfully");
}

AudioPlayer& AudioPlayer::instance() {
    if (!instance_) {
        instance_ = std::shared_ptr<AudioPlayer>();
    }
    return *instance_;
}

void AudioPlayer::playMusic(const std::string& trackName) {
    playSource(musicSource_, musicBuffer_, currentMusic_, trackName);
}

void AudioPlayer::pauseMusic() { pauseSource(musicSource_); }

void AudioPlayer::resumeMusic() { resumeSource(musicSource_); }

void AudioPlayer::stopMusic() { stopSource(musicSource_); }

void AudioPlayer::playSound(const std::string& trackName) {
    playSource(soundSource_, soundBuffer_, currentSound_, trackName);
}

void AudioPlayer::pauseSound() { pauseSource(soundSource_); }

void AudioPlayer::resumeSound() { resumeSource(soundSource_); }

void AudioPlayer::stopSound() { stopSource(soundSource_); }

void AudioPlayer::addAudio(const std::string& filename, const std::string& key) {
    if (audioList_.addAudio(audiofilesPath_ + filename, key)) {
    } else {
        VN_LOGS_ERROR("Audio adding '" + audiofilesPath_ + filename + "' error");
    }
}

void AudioPlayer::removeAudio(const std::string& key) {
    if (audioList_.removeAudio(key)) {
        VN_LOGS_INFO("Audio '" + key + "' has been removed");
    } else {
        VN_LOGS_ERROR("Audio removing '" + key + "' error");
    }
}

void AudioPlayer::setMusicVolume(float volume) {
    if (volume != musicVolume_) {
        alSourcef(musicSource_, AL_GAIN, volume);
        musicVolume_ = volume;
    }
}

void AudioPlayer::setSoundVolume(float volume) {
    if (volume != soundVolume_) {
        alSourcef(soundSource_, AL_GAIN, volume);
        soundVolume_ = volume;
    }
}

float AudioPlayer::musicVolume() { return musicVolume_; }

float AudioPlayer::soundVolume() { return soundVolume_; }

void AudioPlayer::mute() {
    alSourcef(musicSource_, AL_GAIN, 0.0f);
    alSourcef(soundSource_, AL_GAIN, 0.0f);
    isMute_ = true;
}

void AudioPlayer::unmute() {
    alSourcef(musicSource_, AL_GAIN, musicVolume_);
    alSourcef(soundSource_, AL_GAIN, soundVolume_);
    isMute_ = false;
}

bool AudioPlayer::isMute() { return isMute_; }

std::optional<std::string> AudioPlayer::currentMusic() { return currentMusic_; }

std::optional<std::string> AudioPlayer::currentSound() { return currentSound_; }

void AudioPlayer::pushState() {
    // if doesn't play - nullopt - for either

    std::int32_t state = AL_STOPPED;
    alGetSourcei(soundSource_, AL_SOURCE_STATE, &state);

    State s{currentMusic_, currentSound_};
    states_.push_back(std::move(s));

    currentMusic_ = "";
    currentSound_ = "";
    stopMusic();
    stopSound();
}

void AudioPlayer::popState() {
    // if != nullopt - play

    if (states_.empty()) {
        return;
    }

    // State s = states_.back();
    // states_.pop_back();
    // currentMusic_ = s.musicKey;
    // currentSound_ = s.soundKey;

    // turn on music if is playing
    // if (currentMusic_) {
    //     playMusic(*currentMusic_);
    // } else {
    //     musicBuffer_ = audioList_.GetAudio(*currentMusic_);
    //     alSourcei(musicSource_, AL_BUFFER, static_cast<std::int32_t>(musicBuffer_));
    // }

    // set sound
    // soundBuffer_ = audioList_.GetAudio(currentSound_);
    // alSourcei(soundSource_, AL_BUFFER, static_cast<std::int32_t>(soundBuffer_));
}

void AudioPlayer::wipeStates() {
    stopMusic();
    stopSound();
    if (states_.empty()) {
        return;
    }
    states_.clear();
}

void AudioPlayer::setAudiofilePath(const std::string& path) { audiofilesPath_ = path; }

auto AudioPlayer::createDump() -> Dump {
    if (states_.empty()) {
        return Dump{audioList_.dump(), State{std::nullopt, std::nullopt}};
    }

    return Dump{audioList_.dump(), State{states_.back().music, states_.back().sound}};
}

void AudioPlayer::playSource(std::uint32_t source, std::uint32_t& buffer, std::optional<std::string>& current,
                             const std::string& trackName) {
    if (trackName == current) {
        std::int32_t state = AL_STOPPED;
        alGetSourcei(source, AL_SOURCE_STATE, &state);

        if (state == AL_PLAYING) {
            // already playing it
            return;
        }
    }

    std::optional<std::uint32_t> newBufferOpt = audioList_.getAudio(trackName);

    if (!newBufferOpt) {
        VN_LOGS_ERROR("Audiofile '" + trackName + "' is not found (make it sure to add it)");
        return;
    }
    std::uint32_t newBuffer = *newBufferOpt;

    if (newBuffer != buffer) {
        buffer = newBuffer;
        alSourcei(source, AL_BUFFER, static_cast<std::int32_t>(source));
    }
    alSourcePlay(source);
    current = trackName;
}

void AudioPlayer::pauseSource(std::uint32_t source) {
    std::int32_t state = AL_STOPPED;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        alSourcePause(source);
    }
}

void AudioPlayer::resumeSource(std::uint32_t source) {
    std::int32_t state = AL_STOPPED;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state == AL_PAUSED) {
        alSourcePlay(source);
    }
}

void AudioPlayer::stopSource(std::uint32_t source) {
    std::int32_t state = AL_STOPPED;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        alSourceStop(source);
    }
}

} // namespace VNEngine