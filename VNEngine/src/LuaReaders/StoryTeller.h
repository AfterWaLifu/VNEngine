#pragma once

#include "LuaReaders/LuaReader.h"

#include <fstream>

namespace VNEngine {

	class StoryTeller : public LuaReader {

		static inline std::string sScriptsPath;

		std::ifstream m_LuaFile;
		std::string m_CurrentFile;
		int m_CurrentLine;

		bool m_PosLoaded;
		std::streampos m_PosInLua;

		bool m_Go;
		bool m_Skip;
		bool m_Completed;

		void goReadGoReadGo();

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
		void Wait();
		void SetSkip(bool skip);
		bool GetSkip();

		bool GetCompleted();

		std::streampos GetCurrentPos();
		void SetCurrentPos(std::streampos pos);
	};
}