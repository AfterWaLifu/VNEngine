#pragma once

#include <lua.hpp>

#include "LuaWidgetFunctions.h"
#include "LuaSetsFunctions.h"

namespace VNEngine {

	class LuaReader {
	protected:
		lua_State* L;

		void RegisterInterfaceFunctions();
		void RegisterSettingsFunctons();
		void RegisterFSMFunctuins();
	public:
		LuaReader();
		virtual ~LuaReader();
	};
}