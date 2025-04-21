#pragma once

#include <optional>
#include <string>
#include <unordered_map>

namespace VNEngine {

class AudioList {
public:
    AudioList() = default;
    ~AudioList();

    bool addAudio(const std::string& filepath, const std::string& key);
    bool removeAudio(const std::string& key);

    // TODO: change to optional
    std::optional<std::uint32_t> getAudio(const std::string& key);

    std::unordered_map<std::string, std::string> dump();

private:
    std::unordered_map<std::string, std::uint32_t> soundBufferList_;

    void removeFromDumpList(const std::string& key);
    std::unordered_map<std::string, std::string> listOfPathes_;
};

} // namespace VNEngine