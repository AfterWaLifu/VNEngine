#pragma once

#include "LuaReaders/LuaReader.h"

#include <string>

namespace VNEngine {

	class StoryTeller : public LuaReader {

		static inline std::string sScriptsPath;

	public:
		StoryTeller();
		~StoryTeller();

		//
		//	do next func needed
		//

		static void SetScriptPath(const std::string& path);
		static std::string GetScriptsPath();
	};
}