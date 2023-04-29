#include "LuaReader.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaWidgetFunctions.h"

#include <LuaBridge/LuaBridge.h>

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

	void LuaReader::RegisterInterfaceFunctions() {
		using namespace luabridge;

		getGlobalNamespace(L).
			beginNamespace("Widget").
			addFunction("Add",				LW::WidgetAdding).
			addFunction("Update",			LW::WidgetUpdate).
			addFunction("Remove",			LW::WidgetRemove).
			addFunction("GetButtonClicked",	LW::WidgetButtonClicked).
			addFunction("GetButtonFocused",	LW::WidgetButtonFocused).
			addFunction("GetTextboxValue",	LW::WidgetTextboxValue).
			endNamespace();
	}
}