#pragma once

#include <AL/alc.h>

#include <VNEngine/Audio/AudioList.hpp>

#include <memory>
#include <optional>
#include <vector>

namespace VNEngine {

class AudioPlayer {

private:
    AudioPlayer();
    ~AudioPlayer();

public:
    static AudioPlayer& instance();

    // Plays audio on repeat
    void playMusic(const std::string& trackName);
    // Pauses music, may be resumed
    void pauseMusic();
    void resumeMusic();
    // *holy music stops* and returns to the begining of audio
    void stopMusic();

    // Plays a sound once
    void playSound(const std::string& trackName);
    void pauseSound();
    void resumeSound();
    // stops and returns to the begining of audio
    void stopSound();

    void addAudio(const std::string& filename, const std::string& key);
    void removeAudio(const std::string& key);

    // Value in range [0.0;1.0]
    void setMusicVolume(float volume);
    void setSoundVolume(float volume);

    float musicVolume();
    float soundVolume();

    void mute();
    void unmute();
    bool isMute();

    std::optional<std::string> currentMusic();
    std::optional<std::string> currentSound();

    void pushState();
    void popState();
    void wipeStates();

    void setAudiofilePath(const std::string& path);

    struct State {
        std::optional<std::string> music;
        std::optional<std::string> sound;
    };
    struct Dump {
        std::unordered_map<std::string, std::string> listDump;
        State playerState;
    };
    Dump createDump();

private:
    void playSource(std::uint32_t source, std::uint32_t& buffer, std::optional<std::string>& current,
                    const std::string& trackName);
    void pauseSource(std::uint32_t source);
    void resumeSource(std::uint32_t source);
    void stopSource(std::uint32_t source);

    static std::shared_ptr<AudioPlayer> instance_;

    std::string audiofilesPath_;

    AudioList audioList_;

    // OPENAL THINGS BELOW, DONT TOUCH THAT
    ALCdevice* device_;
    ALCcontext* context_;

    std::uint32_t musicSource_;
    std::uint32_t soundSource_;

    std::uint32_t musicBuffer_;
    std::uint32_t soundBuffer_;
    // END OF OPENAL THINGS

    float musicVolume_;
    float soundVolume_;
    bool isMute_;

    std::optional<std::string> currentMusic_;
    std::optional<std::string> currentSound_;

    std::vector<State> states_;
};

#define AP_INSTANCE AudioPlayer::instance()
} // namespace VNEngine