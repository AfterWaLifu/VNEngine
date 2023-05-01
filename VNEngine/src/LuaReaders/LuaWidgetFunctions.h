#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace VNEngine {
	
	namespace LW {

		void WidgetAdding(luabridge::LuaRef t);

		void WidgetUpdate(std::string type, std::string name, luabridge::LuaRef t);

		void WidgetRemove(std::string type, std::string name);

		luabridge::LuaRef WidgetGetState(std::string type, std::string name, lua_State* L);

		bool WidgetButtonClicked(std::string name);

		bool WidgetButtonFocused(std::string name);

		std::string WidgetTextboxValue(std::string name);
	}
}