#include "InterfaceCreator.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaWidgetFunctions.h"
#include "LuaReaders/StoryTeller.h"

#include <LuaBridge/LuaBridge.h>


#define lb luabridge

namespace VNEngine {

	InterfaceCreator::InterfaceCreator() {
		RegisterInterfaceFunctions();
	}

	InterfaceCreator::~InterfaceCreator() {
	}

	void InterfaceCreator::Draw(const std::string& what) {
		if (luaL_dofile(L, (StoryTeller::GetScriptsPath() + "look.lua").c_str())) {
			VN_LOGS_WARNING("Can't open 'look.lua' in a scripts folder");
		}

		lb::LuaRef lr = lb::getGlobal(L, what.c_str());
		if (lr.isTable()) {
			for (int i = 1; !(lr[i].isNil()); ++i) {
				LW::WidgetAdding(lr[i]);
			}
		}
		else {
			VN_LOGS_WARNING("Error on looking for name below in the 'look.lua'" << what);
		}
	}
}