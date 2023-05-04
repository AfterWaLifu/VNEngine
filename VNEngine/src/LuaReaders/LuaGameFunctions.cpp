#include "LuaGameFunctions.h"
#include "vnepch.h"

#include <SDL2/SDL.h>

#include "Core/Logger.h"
#include "Widgets/WidgetsManager.h"
#include "Controls/InputHandler.h"
#include "LuaReaders/StoryTeller.h"

namespace VNEngine {
	
	namespace LG {
		static StoryTeller* s_ST;

		lua_Debug ar;

		void setStoryTeller(StoryTeller* st) {
			s_ST = st;
		}

		void say(std::string what, lua_State* L) {
			auto t = WM_INSTANCE.GetText("say");
			if (t) {
				t->SetText(what);
				lua_getstack(L, 1, &ar);
				lua_getinfo(L, "nSl", &ar);
				s_ST->SetCurrentLineNumber(ar.currentline);
			}
			else {
				VN_LOGS_WARNING("Create 'say' text widget to be able to use 'say'");
			}
			while (!(IH_INSTANCE.getMouseButtonState(LEFT))) {}
		}

		void who(std::string what, lua_State* L) {
			auto t = WM_INSTANCE.GetText("who");
			if (t) {
				t->SetText(what);
				lua_getstack(L, 1, &ar);
				lua_getinfo(L, "nSl", &ar);
				s_ST->SetCurrentLineNumber(ar.currentline);
			}
			else {
				VN_LOGS_WARNING("Create 'who' text widget to be able to use 'who'");
			}
		}

		void wait(uint32_t millesecconds) {
			SDL_Delay(millesecconds);
		}
	}
}