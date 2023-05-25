#pragma once

#include "Widgets/Vectors.h"

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

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
	}
}