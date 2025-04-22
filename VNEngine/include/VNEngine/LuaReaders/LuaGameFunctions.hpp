#pragma once

#include <VNEngine/Widgets/Vectors.hpp>

// clang-format off
// LuaBridge.h HAVE TO be below lua.hpp
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>
// clang-format on

#include <string>

namespace VNEngine {
class Artist;
class StoryTeller;
struct buttonState;

namespace LG {

void setStoryTeller(StoryTeller* st);

void say(std::string what, lua_State* L);

void who(std::string what, lua_State* L);

void wait(uint32_t millesecconds);

void stop();

void choose(luabridge::LuaRef varname, luabridge::LuaRef variants, lua_State* L);

void setDefaultChooseButton(buttonState* bs, bool withGeometry);
void SetDrawwerToGetWindowSize(Artist* partist);
} // namespace LG
} // namespace VNEngine