#pragma once

#include <map>
#include <string>

namespace VNEngine {

	class AudioList
	{
	public:
		AudioList();
		~AudioList();

		bool AddAudio(std::string filename, std::string key);
		bool RemoveAudio(std::string key);

		uint32_t GetAudio(std::string key);

	private:
		std::map<std::string, uint32_t> m_SoundBufferList;
	};
}

