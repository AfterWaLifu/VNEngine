#pragma once

#include <unordered_map>
#include <string>

namespace VNEngine {

	class AudioList
	{
	public:
		AudioList();
		~AudioList();

		/**
		* @brief Adding a new audiofile to the dictionary of its
		* @param filename	The path + name of the file
		* @param key		Keyword to get an access to this audio
		* @return True if added and everything is fine
		*/
		bool AddAudio(std::string filename, std::string key);
		/**
		* @brief Removing audio from the dictionary
		* @param key		Keyword of audio to remove
		* @return True if added and everything is fine
		*/
		bool RemoveAudio(std::string key);

		/**
		* @brief Being used to get an access to the added audio by key
		* @param key		Keyword of needed audio
		*/
		uint32_t GetAudio(std::string key);

		std::vector<std::pair<std::string, std::string>> Dump();

	private:
		/// Dictionary of all the added audio (pairs keyword - internal openal code)
		std::unordered_map<std::string, uint32_t> m_SoundBufferList;
		
		void RemoveFromDumpList(const std::string& key);
		std::vector<std::pair<std::string, std::string>> m_ListOfPathes;	///	To save current state of list
	};
}

