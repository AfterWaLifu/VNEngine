#include "StoryTeller.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaGameFunctions.h"

namespace VNEngine {

	StoryTeller::StoryTeller()
		: m_CurrentFile(""), m_CurrentLine(0), m_Go(true), m_Skip(false), m_PosLoaded(false)
	{
		RegisterInterfaceFunctions();
		RegisterSettingsFunctons();
		RegisterGameFunctions();
		LG::setStoryTeller(this);
	}

	StoryTeller::~StoryTeller() {
	}

	void StoryTeller::parseFile() {
		if (m_LuaFile.eof()) return;

		size_t linenum = 0;
		char buffer[256];
		while (!m_LuaFile.eof() && m_LuaFile.getline(buffer, 256, '\n')) {
			++linenum;
			auto line = std::string(buffer);
			if (line.empty()) continue;
			if (int dots = (int)line.find("::") != std::string::npos) {
				if (int dote = (int)line.find("::", dots + 3)) {
					auto mark = line.substr(dots+1, dote - dots - 1);
					if (mark.empty()) continue;
					m_gotoMarks.push_back({ m_LuaFile.tellg(),linenum, mark});
				}
			}
		}
		m_LuaFile.seekg(0);
	}

	bool StoryTeller::handleJump(const std::string& line) {
		bool jumped = false;
		if (int gotopos = (int)line.find("goto ") != std::string::npos) {
			std::string mark;
			int markendpos = (int)line.find(" ", gotopos+4);
			if (markendpos == std::string::npos) {
				mark = line.substr(gotopos + 4);
			}
			else {
				mark = line.substr(gotopos + 4, markendpos - gotopos - 4);
			}
			for (const auto& m : m_gotoMarks) {
				if (m.name == mark) {
					m_LuaFile.seekg(m.pos);
					m_PosInLua = m.linenum;
					jumped = true;
				}
			}
			if (!jumped) {
				VN_LOGS_WARNING("Error on 'goto' at line below" << 
					std::to_string(m_CurrentLine) + " | " + line);
			}
		}
		return jumped;
	}

	void StoryTeller::goReadGoReadGo() {
		char buffer[256];
		while (m_Go && !m_Choosing && m_LuaFile.getline(buffer, 256, '\n')) {
			++m_CurrentLine;
			m_PosInLua = m_LuaFile.tellg();
			auto error = luaL_dostring(L, buffer);
			if (handleJump(std::string(buffer))) continue;
			if (error) {
				VN_LOGS_WARNING("Lua error in line below" <<
					std::to_string(m_CurrentLine) + " | " + buffer);
				VN_LOGS_WARNING(lua_tostring(L,-1));
				lua_pop(L,1);
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

		parseFile();

		if (m_PosLoaded) {
			m_LuaFile.seekg(m_PosInLua);
			m_PosLoaded = false;
			Wait();
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
		return (int)m_CurrentLine;
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
			addFunction("wait", LG::wait).
			addFunction("stop", LG::stop).
			addFunction("choose", LG::choose);
	}

	void StoryTeller::Go() {
		if (m_Choosing) return;
		m_Go = true;
		goReadGoReadGo();
	}

	void StoryTeller::Wait() {
		m_Go = false;
	}

	void StoryTeller::Choosing(bool sure) {
		m_Choosing = sure;
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
		m_PosLoaded = true;
		m_PosInLua = pos;
	}
}