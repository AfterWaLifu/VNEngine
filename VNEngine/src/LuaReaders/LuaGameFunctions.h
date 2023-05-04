#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

#include <string>

namespace VNEngine {
	class StoryTeller;

	namespace LG {
		
		void setStoryTeller(StoryTeller* st);

		void say(std::string what, lua_State* L);

		void who(std::string what, lua_State* L);

		void wait(uint32_t millesecconds);
	}
}