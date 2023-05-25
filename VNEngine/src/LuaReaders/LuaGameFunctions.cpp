#include "LuaGameFunctions.h"
#include "vnepch.h"

#include <SDL2/SDL.h>

#include "Core/Logger.h"
#include "Graphic/Artist.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/StoryTeller.h"

namespace VNEngine {
	
	namespace LG {
		static StoryTeller* s_ST;
		static buttonState s_bs = {
				{{ 0,0,0,0 }, "","",
				"", {0,0,0,255}, {255,255,255,128},{ 215,215,215,255 },
				4 | 8, true, true,true,true,3,3 },
				{215,215,215,255},{175,175,175,255}, nullptr, nullptr };
		static bool s_bs_geomsetled = false;
		static Artist* s_pArtist;

		void setStoryTeller(StoryTeller* st) {
			s_ST = st;
		}

		void say(std::string what, lua_State* L) {
			auto t = WM_INSTANCE.GetText("say");
			if (t) {
				t->SetText(what);
			}
			else {
				VN_LOGS_WARNING("Create 'say' text widget to be able to use 'say'");
			}

			s_ST->Wait();
		}

		void who(std::string what, lua_State* L) {
			auto t = WM_INSTANCE.GetText("who");
			if (t) {
				t->SetText(what);
			}
			else {
				VN_LOGS_WARNING("Create 'who' text widget to be able to use 'who'");
			}
		}

		void wait(uint32_t millesecconds) {
			SDL_Delay(millesecconds);
		}

		void stop() {
			s_ST->Wait();
		}
		
		void choose(luabridge::LuaRef varname, luabridge::LuaRef variants, lua_State* L) {
			if (varname.isString() && variants.isTable()) {
				s_ST->Choosing();
				int count = variants.length();
				const auto var = varname.tostring();
				vec2 windowgeometry = s_pArtist->GetWindowSize();
				float offset;
				if (WM_INSTANCE.ExistsText("say") && WM_INSTANCE.GetText("say")->IsItShown())
					windowgeometry.y = WM_INSTANCE.GetText("say")->GetGeometry().y;

				if (!s_bs_geomsetled) {
					s_bs.ts.geometry.w = windowgeometry.x / 3;
					s_bs.ts.geometry.h = 40;
				}
				s_bs.ts.geometry.x = (windowgeometry.x - s_bs.ts.geometry.w) / 2;
				offset = (float)count / 2.0f * (float)s_bs.ts.geometry.h;
				
				for (int i = 1; i <= count; ++i) {
					s_bs.function = [L, var, i, count]() {
						luabridge::setGlobal(L, i, var.c_str());
						s_ST->Choosing(false);
						s_ST->Go();
						for (int j = 1; j <= count; ++j)
							WM_INSTANCE.RemoveWidget(WidgetsManager::WIDGET_BUTTON,
								std::string("choise") + std::to_string(j));
					};
					s_bs.ts.geometry.y = (int)(windowgeometry.y / 2 + (offset * ((float)i - ((float)count)/2)));
					s_bs.ts.text = variants[i].tostring();
					WM_INSTANCE.AddWidget(std::string("choise")+std::to_string(i), new Button(s_bs));
				}
			}
			else {
				VN_LOGS_WARNING("Attemp to use 'choose' a wrong way at in a file and line below" <<
					s_ST->GetCurrentFileName() << std::to_string(s_ST->GetCurrentLineNumber())+ " line");
			}
		}
		
		void setDefaultChooseButton(buttonState* bs, bool withGeometry) {
			s_bs.ts = bs->ts;
			s_bs.defaultborder = bs->defaultborder;
			s_bs.focusborder = bs->focusborder;
			s_bs_geomsetled = withGeometry;
		}	
		void SetDrawwerToGetWindowSize(Artist* partist) {
			s_pArtist = partist;
		}
	}
}