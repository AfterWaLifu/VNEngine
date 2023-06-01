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

		size_t linenum = 0, start = 0;
		int nestedlvl = 0;
		char buffer[256];
		while (!m_LuaFile.eof() && m_LuaFile.getline(buffer, 256, '\n')) {
			++linenum;
			auto line = std::string(buffer);
			size_t onemoreelseifinline = 0;

			if (line.empty()) continue;
			if (start = line.find("if") != std::string::npos &&
				line.find("elseif") != start - 4) {
				++nestedlvl;
				m_ifMarks.push_back({ {m_LuaFile.tellg(),linenum} });
			}
			{
			elseifcheck:
				if ((start = line.find("elseif", onemoreelseifinline)) != std::string::npos) {
					if (nestedlvl) {
						m_ifMarks[m_ifMarks.size() - nestedlvl].elseifs.push_back({ { m_LuaFile.tellg(),linenum }, line.length()});
						onemoreelseifinline = line.find("then", start + 6);
						goto elseifcheck;
					}
					else {
						VN_LOGS_WARNING("Extra 'elseif' without 'if' at line " + std::to_string(linenum));
					}
				}
			}
			if (start = line.find("else", onemoreelseifinline) != std::string::npos) {
				m_ifMarks[m_ifMarks.size() - nestedlvl].another = { m_LuaFile.tellg(), linenum };
			}
			if (start = line.find("end") != std::string::npos) {
				m_ifMarks[m_ifMarks.size() - nestedlvl].end = { m_LuaFile.tellg(), linenum };
				--nestedlvl;
			}
			if (size_t dots = line.find("::") != std::string::npos) {
				if (size_t dote = line.find("::", dots + 3)) {
					auto mark = line.substr(dots + 1, dote - dots - 1);
					if (mark.empty()) continue;
					m_gotoMarks.push_back({ {m_LuaFile.tellg(),linenum}, mark });
				}
			}
		}
		if (nestedlvl) 
			VN_LOGS_WARNING("Error on reading game script file, 'end' operand expected "
				+ std::to_string(nestedlvl) + " more time(s)");
		m_LuaFile.seekg(0);
	}

	bool StoryTeller::handleJump(const std::string& line) {
		bool jumped = false;
		size_t gotopos;
		if ((gotopos = line.find("goto ")) != std::string::npos) {
			std::string mark = line.substr(gotopos + 5);
			for (const auto& m : m_gotoMarks) {
				if (m.name == mark) {
					m_LuaFile.seekg(m.m.pos);
					m_PosInLua = m.m.linenum;
					m_CurrentLine = m.m.linenum;
					jumped = true;
					break;
				}
			}
			if (!jumped) {
				VN_LOGS_WARNING("Error on 'goto' at line below" << 
					std::to_string(m_CurrentLine) + " | " + line);
			}
		}
		return jumped;
	}

	bool StoryTeller::handleIf(const std::string& line) {
		bool jumped = false;
		size_t pos;
		ifmark* mark = nullptr;
		uint8_t what = 0;
		bool found = false;

		for (auto& m : m_ifMarks) {
			if (m.start.linenum == m_CurrentLine) {
				mark = &m;
				what = 255;
				break;
			}
			if (m.another.linenum == m_CurrentLine) {
				mark = &m;
				what = 254;
				break;
			}
			if (m.end.linenum == m_CurrentLine) {
				mark = &m;
				what = 253;
				break;
			}
			if (m.elseifs.size() == 0) continue;
			for (uint8_t i = 0; i < m.elseifs.size() && i < 253; ++i) {
				if (m.elseifs[i].m.linenum == m_CurrentLine) {
					mark = &m;
					what = i;
					found = true;
					break;
				}
			}
			if (found) { found = false; break; }
		}

		if (mark) {
			auto pos_before_anything = m_LuaFile.tellg();
			bool came_as_it_is = true;
			if (what == 255) {
				std::string condition = "";
				size_t thenpos = 0;
				pos = line.find("if");
				thenpos = line.find("then", pos + 2);
				if (thenpos - pos <= 2) goto end;
				condition = line.substr(pos + 2, thenpos - pos - 2);
				luaL_loadstring(L, (std::string("return ") + condition).c_str());
				if (lua_pcall(L, 0, 1, 0)) {
					VN_LOGS_WARNING("Error on condition for if in line " + std::to_string(mark->start.linenum));
					VN_LOGS_WARNING(lua_tostring(L, -1));
					lua_pop(L, 1);
					goto end;
				}
				bool result = lua_toboolean(L, -1);
				lua_pop(L, 1);
				if (result) {
					if (mark->end.linenum == mark->start.linenum) {
						if (mark->elseifs.size()) {
							pos = line.find("else", thenpos);
						}
						else {
							pos = line.find("end", thenpos);
						}
						condition = line.substr(thenpos + 4, pos - thenpos - 4);
						luaL_dostring(L, condition.c_str());
						if (m_LuaFile.tellg() == pos_before_anything) {
							m_LuaFile.seekg(mark->end.pos);
							m_PosInLua = mark->end.pos;
							m_CurrentLine = mark->end.linenum;
						}
						jumped = true;
						goto end;
					}
					else {
						m_LuaFile.seekg(mark->start.pos);
						m_PosInLua = mark->start.pos;
						m_CurrentLine = mark->start.linenum;
						jumped = true;
						goto end;
					}
				}
				else if (mark->elseifs.size()) {
				samelineelseifcheck:
					came_as_it_is = false;
					pos = line.find("elseif", pos);
					if (pos == std::string::npos) goto luaelsemark;
					thenpos = line.find("then", pos+6);
					if (thenpos - pos <= 6) goto end;
					condition = line.substr(pos+6, thenpos-pos-6);
					luaL_loadstring(L, (std::string("return ") + condition).c_str());
					if (lua_pcall(L, 0, 1, 0)) {
						VN_LOGS_WARNING("Error on condition for elseif in line " + std::to_string(mark->start.linenum));
						VN_LOGS_WARNING(lua_tostring(L, -1));
						lua_pop(L, 1);
						goto end;
					}
					bool result = lua_toboolean(L,-1);
					lua_pop(L, 1);
					if (result) {
						pos = line.find("else", thenpos);
						if (pos == -1) pos = line.find("end", thenpos);
						if (pos == -1) {
							condition = line.substr(thenpos + 4);
						}
						else {
							condition = line.substr(thenpos + 4, pos-thenpos-4);
						}
						luaL_dostring(L, condition.c_str());
						if (m_LuaFile.tellg() == pos_before_anything) {
							m_LuaFile.seekg(mark->end.pos);
							m_PosInLua = mark->end.pos;
							m_CurrentLine = mark->end.linenum;
						}
						jumped = true;
						goto end;
					}
					else goto samelineelseifcheck;

					goto end;
				}
				else {
				luaelsemark:
					if (mark->another.linenum == mark->start.linenum && mark->another.pos == 0) {
						m_LuaFile.seekg(mark->end.pos);
						m_PosInLua = mark->end.pos;
						m_CurrentLine = mark->end.linenum;
						jumped = true;
						goto end;
					};
					if (mark->another.linenum == mark->start.linenum) {
						pos = line.find("end", thenpos);
						condition = line.substr(thenpos + 4, pos - thenpos - 4);
						luaL_dostring(L, condition.c_str());
						m_LuaFile.seekg(mark->end.pos);
						m_PosInLua = mark->end.pos;
						m_CurrentLine = mark->end.linenum;
						jumped = true;
						goto end;
					}
					else if (mark->another.pos != 0){
						m_LuaFile.seekg(mark->another.pos);
						m_PosInLua = mark->another.pos;
						m_CurrentLine = mark->another.linenum;
						jumped = true;
						goto end;
					}
					else {
						for (const auto m : mark->elseifs) {
							if (m.m.linenum > m_CurrentLine) {
								m_LuaFile.seekg((size_t)m.m.pos - m.length);
								m_PosInLua = m_LuaFile.tellg();
								m_CurrentLine = m.m.linenum;
								jumped = true;
								goto end;
							}
						}
					}
				}
			}
			else if (what == 254) {
				m_LuaFile.seekg(mark->another.pos);
				m_PosInLua = mark->another.pos;
				m_CurrentLine = mark->another.linenum;
				jumped = true;
				goto end;
			}
			else if (what == 253) {
			handleendofif:
				m_LuaFile.seekg(mark->end.pos);
				m_PosInLua = mark->end.pos;
				m_CurrentLine = mark->end.linenum;
				jumped = true;
				goto end;
			}
			else {
				if (came_as_it_is) goto handleendofif;
				std::string condition = "";
				size_t thenpos = 0;
				pos = line.find("elseif");
				thenpos = line.find("then", pos + 6);
				if (thenpos - pos <= 2) goto end;
				condition = line.substr(pos + 6, thenpos - pos - 6);
				luaL_loadstring(L, (std::string("return ") + condition).c_str());
				if (lua_pcall(L, 0, 1, 0)) {
					VN_LOGS_WARNING("Error on condition for in in line " + std::to_string(mark->start.linenum));
					VN_LOGS_WARNING(lua_tostring(L, -1));
					lua_pop(L, 1);
					goto end;
				}
				bool result = lua_toboolean(L, -1);
				lua_pop(L, 1);
				if (result) {
					m_LuaFile.seekg(mark->elseifs[what].m.pos);
					m_PosInLua = mark->elseifs[what].m.pos;
					m_CurrentLine = mark->elseifs[what].m.linenum;
					jumped = true;
					goto end;
				}
				else {
					if (mark->elseifs.size() > what) {
						auto pos = (size_t)mark->elseifs[what + 1].m.pos - mark->elseifs[what + 1].length;
						m_LuaFile.seekg(pos);
						m_PosInLua = pos;
						m_CurrentLine = mark->elseifs[what+1].m.linenum;
						jumped = true;
						goto end;
					}
					else if (mark->another.pos != 0) {
						m_LuaFile.seekg(mark->another.pos);
						m_PosInLua = mark->another.pos;
						m_CurrentLine = mark->another.linenum;
						jumped = true;
						goto end;
					}
					else {
						m_LuaFile.seekg(mark->end.pos);
						m_PosInLua = mark->end.pos;
						m_CurrentLine = mark->end.linenum;
						jumped = true;
						goto end;
					}
				}
			}
		}

	end:
		return jumped;
	}

	void StoryTeller::goReadGoReadGo() {
		char buffer[256];
		while (m_Go && !m_Choosing && m_LuaFile.getline(buffer, 256, '\n')) {
 			++m_CurrentLine;
			m_PosInLua = m_LuaFile.tellg();
			auto error = luaL_dostring(L, buffer);
			if (handleJump(std::string(buffer))) continue;
			if (handleIf(std::string(buffer))) continue;

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