#include "InterfaceCreator.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaWidgetFunctions.h"
#include "LuaReaders/StoryTeller.h"
#include "Widgets/WidgetsManager.h"

#include <LuaBridge/LuaBridge.h>


#define lb luabridge

namespace VNEngine {

	InterfaceCreator::InterfaceCreator() {
		RegisterInterfaceFunctions();
		RegisterSettingsFunctons();
		RegisterFSMFunctuins();
	}

	InterfaceCreator::~InterfaceCreator() {
	}

	void InterfaceCreator::Draw(const std::string& what) {
		WM_INSTANCE.WipeWidgets();

		if (luaL_dofile(L, (StoryTeller::GetScriptsPath() + "look.lua").c_str())) {
			VN_LOGS_WARNING("Can't open 'look.lua' in a scripts folder");
		}

		lb::LuaRef lr = lb::getGlobal(L, what.c_str());
		if (lr.isFunction()) {
			lr();
		}
		else {
			VN_LOGS_WARNING("Error on looking for name below in the 'look.lua'" << what);
		}
	}
}