#pragma once

#include <lua.hpp>

namespace VNEngine {

	class LuaReader {
	protected:
		lua_State* L;

	public:
		LuaReader();
		virtual ~LuaReader();
	};
}