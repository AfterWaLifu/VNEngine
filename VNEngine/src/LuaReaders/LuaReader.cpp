#include "LuaReader.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	LuaReader::LuaReader() {
		L = luaL_newstate();

		if (!L) {
			VN_LOGS_ERROR("Can't init Lua state, can't work >.<");
			return;
		}
		
		luaL_openlibs(L);
		lua_pcall(L, 0, 0, 0);
	}

	LuaReader::~LuaReader() {
		lua_close(L);
	}
}