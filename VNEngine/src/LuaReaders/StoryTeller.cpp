#include "StoryTeller.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaGameFunctions.h"

namespace VNEngine {

	StoryTeller::StoryTeller()
		: m_CurrentFile(""), m_CurrentLine(0)
	{
		RegisterInterfaceFunctions();
		RegisterSettingsFunctons();
		RegisterGameFunctions();
		LG::setStoryTeller(this);
	}

	StoryTeller::~StoryTeller() {
	}

	void StoryTeller::DoFile(const std::string& filename) {
		m_CurrentFile = filename;
		if (luaL_dofile(L, (sScriptsPath + m_CurrentFile).c_str()) != 0) {
			VN_LOGS_ERROR("Lua file reading abortion" << m_CurrentFile <<
				std::string("At line ") + std::to_string(m_CurrentLine));
		}
	}

	std::string StoryTeller::GetCurrentFileName() {
		return m_CurrentFile;
	}

	void StoryTeller::SetCurrentLineNumber(int lineNumber) {
		m_CurrentLine = lineNumber;
	}

	int StoryTeller::GetCurrentLineNumber() {
		return m_CurrentLine;
	}

	void StoryTeller::SetScriptPath(const std::string& path) {
		sScriptsPath = path;
	}

	std::string StoryTeller::GetScriptsPath() {
		return sScriptsPath;
	}

	void StoryTeller::RegisterGameFunctions() {
		using namespace luabridge;

		getGlobalNamespace(L).
			addFunction("say", LG::say).
			addFunction("who", LG::who).
			addFunction("wait", LG::wait);
	}

}