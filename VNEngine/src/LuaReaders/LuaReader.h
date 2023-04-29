#pragma once

#include <lua.hpp>

namespace VNEngine {

	class LuaReader {
	protected:
		lua_State* L;

		void RegisterInterfaceFunctions();
	public:
		LuaReader();
		virtual ~LuaReader();
	};
}