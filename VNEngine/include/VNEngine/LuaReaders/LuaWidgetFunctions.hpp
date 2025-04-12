#pragma once

// clang-format off
// LuaBridge.h HAVE TO be below lua.hpp
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
// clang-format on

namespace VNEngine {

namespace LW {

void WidgetAdding(luabridge::LuaRef t);

void WidgetUpdate(luabridge::LuaRef t);

void WidgetRemove(std::string type, std::string name);

luabridge::LuaRef WidgetGetState(std::string type, std::string name, lua_State* L);

bool WidgetButtonClicked(std::string name);

bool WidgetButtonFocused(std::string name);

std::string WidgetTextboxValue(std::string name);

void SetDefaultChoiseButton(luabridge::LuaRef t);
} // namespace LW
} // namespace VNEngine