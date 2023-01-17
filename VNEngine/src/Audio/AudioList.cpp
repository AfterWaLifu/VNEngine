#include "vnepch.h"
#include "AudioList.h"

#include <AL/al.h>
#include <AL/alext.h>
#include <sndfile.h>

#include <inttypes.h>

#include "Core/Logger.h"

namespace VNEngine {
	
	AudioList::AudioList()
	{
	}

	AudioList::~AudioList() {
		if (m_SoundBufferList.size() == 0) return;

		std::vector<unsigned int> buffers;
		for (auto start = m_SoundBufferList.begin(); start != m_SoundBufferList.end(); ++start) {
			buffers.push_back((*start).second);
		}
		alDeleteBuffers((ALsizei)buffers.size(), buffers.data());
		m_SoundBufferList.clear();
	}
	
	bool AudioList::AddAudio(std::string filename, std::string key) {
        ALenum err, format;
        ALuint buffer;
        SNDFILE* sndfile;
        SF_INFO sfinfo;
        short* membuf;
        sf_count_t num_frames;
        ALsizei num_bytes;

        logs.setMeta("ERROR");

        sndfile = sf_open(filename.c_str(), SFM_READ, &sfinfo);
        if (!sndfile)
        {
            logs << "Could not open audio in" << filename << " (" << sf_strerror(sndfile) << ")";
            return false;
        }
        if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
        {
            logs << "Bad sample count in " << filename << "(" << sfinfo.frames << ")";
            sf_close(sndfile);
            return false;
        }

        format = AL_NONE;
        if (sfinfo.channels == 1)
            format = AL_FORMAT_MONO16;
        else if (sfinfo.channels == 2)
            format = AL_FORMAT_STEREO16;
        else if (sfinfo.channels == 3)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT2D_16;
        }
        else if (sfinfo.channels == 4)
        {
            if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
                format = AL_FORMAT_BFORMAT3D_16;
        }
        if (!format)
        {
            logs << "Unsupported channel count: " << sfinfo.channels << "(" << filename << ")";
            sf_close(sndfile);
            return 0;
        }

        membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

        num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
        if (num_frames < 1)
        {
            free(membuf);
            sf_close(sndfile);
            logs << "Failed to read samples in " << filename << " (" << num_frames << ")";
            return false;
        }
        num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

        buffer = 0;
        alGenBuffers(1, &buffer);
        alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

        free(membuf);
        sf_close(sndfile);

        err = alGetError();
        if (err != AL_NO_ERROR)
        {
            logs << "OpenAL Error: " << alGetString(err);
            if (buffer && alIsBuffer(buffer))
                alDeleteBuffers(1, &buffer);
            return 0;
        }

        logs.setMeta("INFO");
        m_SoundBufferList[key] = buffer;
        return true;
	}
	
	uint32_t AudioList::GetAudio(std::string key)
	{
		if (m_SoundBufferList.find(key) == m_SoundBufferList.end()) {
			return UINT32_MAX;
		}
		else {
			return m_SoundBufferList[key];
		}
	}

	bool AudioList::RemoveAudio(std::string key) {
		auto it = m_SoundBufferList.begin();
		while (it != m_SoundBufferList.end()) {
			if ((*it).first == key) {
				alDeleteBuffers(1, &((*it).second));
				it = m_SoundBufferList.erase(it);
				return true;
			}
			++it;
		}
		return false;
	}
}