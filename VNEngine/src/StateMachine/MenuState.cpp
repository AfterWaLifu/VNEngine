#include "MenuState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/ReadingState.h"
#include "Widgets/WidgetsManager.h"
#include "Audio/AudioPlayer.h"
#include "StateMachine/SaveLoad.h"

namespace VNEngine {
	
	MenuState::MenuState(const std::string& screen) : m_ScreenToStart(screen), m_MenuState(screen) {
	}

	void MenuState::Handle() {
		if (IH_INSTANCE.getMouseButtonState(RIGHT) && SM_INSTANCE.isThereAReading()) {
			SM_INSTANCE.PopState();
		}
	}
	
	void MenuState::Update() {
	}
	
	void MenuState::Render() {
	}

	void MenuState::Redraw(const std::string& screen) {
		WM_INSTANCE.WipeWidgets();
		m_ScreenToStart = screen;
		m_ic.Draw(m_ScreenToStart);
	}
	
	bool MenuState::onEnter() {
		m_ic.Draw(m_ScreenToStart);

		return true;
	}

	bool MenuState::onExit() {
		WM_INSTANCE.WipeWidgets();
		AP_INSTANCE.StopMusic();
		s_pDrawer->WipeDrawing();
		return true;
	}

	std::string MenuState::GetStateId() const {
		return m_MenuState;
	}
}