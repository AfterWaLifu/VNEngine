#pragma once

#include "Widgets/WidgetsManager.h"

#include <LuaBridge/LuaBridge.h>

namespace VNEngine {
	
	namespace LW {

		void WidgetAdding(luabridge::LuaRef t) {
			using namespace luabridge;
		}

		void WidgetUpdate(std::string type, std::string name, luabridge::LuaRef t) {
			using namespace luabridge;
		}

		void WidgetRemove(std::string type, std::string name) {
			using namespace luabridge;
		}

		bool WidgetButtonClicked(std::string name) {
			return WM_INSTANCE.GetButton(name)->Pressed();
		}

		bool WidgetButtonFocused(std::string name) {
			return WM_INSTANCE.GetButton(name)->Focused();
		}

		std::string WidgetTextboxValue(std::string name) {
			return WM_INSTANCE.GetTextBox(name)->Value();
		}
	}
}