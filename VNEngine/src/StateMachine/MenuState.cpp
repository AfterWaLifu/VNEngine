#include "MenuState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/ReadingState.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/InterfaceCreator.h"

namespace VNEngine {

	MenuState::MenuState(const std::string& screen) : m_ScreenToStart(screen) {
	}
	
	void MenuState::Handle() {
		if (WM_INSTANCE.ExistsButton("start")) {
			if (WM_INSTANCE.GetButton("start")->Pressed()) {
				SM_INSTANCE.ChangeState(new ReadingState());
			}
		}
		//todo add load screen and settings screen
		if (WM_INSTANCE.ExistsButton("exit")) {
			if (WM_INSTANCE.GetButton("exit")->Pressed()) {
				IH_INSTANCE.exit();
			}
		}
	}
	
	void MenuState::Update() {
	}
	
	void MenuState::Render() {
	}
	
	bool MenuState::onEnter() {
		m_MenuState = "menu";
		InterfaceCreator ic;
		ic.Draw(m_ScreenToStart);

		return true;
	}

	bool MenuState::onExit() {
		WM_INSTANCE.WipeWidgets();
		return true;
	}

	std::string MenuState::GetStateId() const {
		return m_MenuState;
	}
}