#include <VNEngine/Audio/AudioList.hpp>

#include <AL/al.h>
#include <AL/alext.h>
#include <sndfile.h>

#include <VNEngine/Core/Logger.hpp>

namespace VNEngine {

AudioList::~AudioList() {
    if (soundBufferList_.empty()) {
        return;
    }

    std::vector<std::uint32_t> buffers;
    for (auto it = soundBufferList_.begin(); it != soundBufferList_.end(); ++it) {
        buffers.push_back(it->second);
    }
    alDeleteBuffers(static_cast<ALsizei>(buffers.size()), buffers.data());
}

bool AudioList::addAudio(const std::string& filepath, const std::string& key) {
    if (soundBufferList_.find(key) != soundBufferList_.end()) {
        auto it = listOfPathes_.find(filepath);
        if (it != listOfPathes_.end() && it->second == key) {
            // already added = success
            return true;
        }
        VN_LOGS_ERROR("Can't add audio - key already taken");
        return false;
    }

    // SNDFILE LIB usage from example as it is, with C-style casts, didn't touch on purpose

    // TODO: UB on adding approximately 2^32 audio files (buffers in uint32), gonna check what sndfile does with it

    ALenum err, format;
    ALuint buffer;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;

    sndfile = sf_open(filepath.c_str(), SFM_READ, &sfinfo);
    if (!sndfile) {
        VN_LOGS_ERROR("Could not open audio in" + filepath + " (" + sf_strerror(sndfile) + ")");
        return false;
    }
    if (sfinfo.frames < 1 || sfinfo.frames > (sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels) {
        VN_LOGS_ERROR("Bad sample count in " + filepath + "(" + (const char*)sfinfo.frames + ")");
        sf_close(sndfile);
        return false;
    }

    format = AL_NONE;
    if (sfinfo.channels == 1)
        format = AL_FORMAT_MONO16;
    else if (sfinfo.channels == 2)
        format = AL_FORMAT_STEREO16;
    else if (sfinfo.channels == 3) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT2D_16;
    } else if (sfinfo.channels == 4) {
        if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
            format = AL_FORMAT_BFORMAT3D_16;
    }
    if (!format) {
        VN_LOGS_ERROR("Unsupported channel count: " << sfinfo.channels << "(" << filepath << ")");
        sf_close(sndfile);
        return false;
    }

    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if (num_frames < 1) {
        free(membuf);
        sf_close(sndfile);
        VN_LOGS_ERROR("Failed to read samples in " + filepath + " (" + (const char*)num_frames + ")");
        return false;
    }
    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

    buffer = 0;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    err = alGetError();
    if (err != AL_NO_ERROR) {
        const std::string message = "OpenAL Error: ";
        const char* error = alGetString(err);
        VN_LOGS_ERROR(message + error);
        if (buffer && alIsBuffer(buffer))
            alDeleteBuffers(1, &buffer);
        return false;
    }

    soundBufferList_.insert({key, buffer});
    listOfPathes_.insert({filepath, key});
    return true;
}

std::optional<std::uint32_t> AudioList::getAudio(const std::string& key) {
    if (soundBufferList_.find(key) == soundBufferList_.end()) {
        return std::nullopt;
    } else {
        return soundBufferList_[key];
    }
}

std::unordered_map<std::string, std::string> AudioList::dump() { return listOfPathes_; }

void AudioList::removeFromDumpList(const std::string& key) {
    auto it = listOfPathes_.begin();
    while (it != listOfPathes_.end()) {
        if ((*it).second == key) {
            listOfPathes_.erase(it);
            return;
        }
        ++it;
    }
}

bool AudioList::removeAudio(const std::string& key) {
    if (listOfPathes_.empty()) {
        return true;
    }

    auto it = soundBufferList_.find(key);
    if (it != soundBufferList_.end()) {
        removeFromDumpList(key);
        alDeleteBuffers(1, &(it->second));
        soundBufferList_.erase(it);
        return true;
    }

    return false;
}
} // namespace VNEngine