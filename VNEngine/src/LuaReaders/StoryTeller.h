#pragma once

#include "LuaReaders/LuaReader.h"

#include <fstream>
#include <vector>

namespace VNEngine {

	class StoryTeller : public LuaReader {

		static inline std::string sScriptsPath;

		std::ifstream m_LuaFile;
		std::string m_CurrentFile;
		size_t m_CurrentLine;

		struct mark {
			std::streampos pos;
			size_t linenum;
		};
		struct gotomark {
			mark m;
			std::string name;
		};
		struct elseifmark {
			mark m;
			size_t length;
		};
		struct ifmark {
			mark start, another, end;
			std::vector<elseifmark> elseifs;
		};
		std::vector<gotomark> m_gotoMarks;
		std::vector<ifmark> m_ifMarks;

		bool m_PosLoaded;
		std::streampos m_PosInLua;

		bool m_Go;
		bool m_Skip;
		bool m_Choosing;
		bool m_Completed;
		bool m_OnTheComment;
		std::vector<bool> m_OnTheIf;

		void parseFile();
		bool handleJump(const std::string& line);
		bool handleIf(const std::string& line);
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
		void Choosing(bool sure = true);
		void SetSkip(bool skip);
		bool GetSkip();

		bool GetCompleted();

		std::streampos GetCurrentPos();
		void SetCurrentPos(std::streampos pos);

		lua_State* getl();
	};
}