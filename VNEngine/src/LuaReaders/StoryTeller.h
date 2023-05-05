#pragma once

#include "LuaReaders/LuaReader.h"

namespace VNEngine {

	class StoryTeller : public LuaReader {

		static inline std::string sScriptsPath;

		std::string m_CurrentFile;
		int m_CurrentLine;

		bool m_Skip;

	public:
		StoryTeller();
		~StoryTeller();

		void DoFile(const std::string& filename);
		std::string GetCurrentFileName();
		void SetCurrentLineNumber(int lineNumber);
		int GetCurrentLineNumber();

		static void SetScriptPath(const std::string& path);
		static std::string GetScriptsPath();

		void RegisterGameFunctions();

		void Go();
		void SetSkip(bool skip);
		bool GetSkip();
	};
}