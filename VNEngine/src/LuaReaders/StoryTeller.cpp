#include "StoryTeller.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaGameFunctions.h"

namespace VNEngine {

	StoryTeller::StoryTeller()
		: m_CurrentFile(""), m_CurrentLine(0), m_Go(true), m_Skip(false)
	{
		RegisterInterfaceFunctions();
		RegisterSettingsFunctons();
		RegisterGameFunctions();
		LG::setStoryTeller(this);
	}

	StoryTeller::~StoryTeller() {
	}

	void StoryTeller::goReadGoReadGo() {
		char buffer[256];
		while (m_Go && m_LuaFile.getline(buffer, 256, '\n')) {
			++m_CurrentLine;
			m_PosInLua = m_LuaFile.tellg();
			auto error = luaL_dostring(L, buffer);
			if (error) {
				VN_LOGS_WARNING("Lua error in line below" <<
					std::to_string(m_CurrentLine) + " | " + buffer);
			}
		}
		if (m_LuaFile.eof()) m_Completed = true;
	}

	void StoryTeller::DoFile(const std::string& filename) {
		m_CurrentFile = filename;
		m_CurrentLine = 0;
		m_LuaFile.open(sScriptsPath + m_CurrentFile);
		
		if (!m_LuaFile.is_open()) {
			VN_LOGS_ERROR("Can't open game file" << sScriptsPath + m_CurrentFile);
		}
		goReadGoReadGo();
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

	void StoryTeller::Go() {
		m_Go = true;
		goReadGoReadGo();
	}

	void StoryTeller::Wait() {
		m_Go = false;
	}
	
	void StoryTeller::SetSkip(bool skip) {
		m_Skip = skip;
		if (m_Skip) Go();
	}

	bool StoryTeller::GetSkip() {
		return m_Skip;
	}
	
	bool StoryTeller::GetCompleted() {
		return m_Completed;
	}
	
	std::streampos StoryTeller::GetCurrentPos() {
		return m_PosInLua;
	}
	
	void StoryTeller::SetCurrentPos(std::streampos pos) {
		m_LuaFile.seekg(pos);
		m_PosInLua = pos;
	}
}