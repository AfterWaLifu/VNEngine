#include "MenuState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/ReadingState.h"
#include "Widgets/WidgetsManager.h"
#include "Audio/AudioPlayer.h"
#include "StateMachine/SaveLoad.h"

namespace VNEngine {
	
	MenuState::MenuState(const std::string& screen) : m_ScreenToStart(screen) {
	}

	void MenuState::Handle() {
		if ((IH_INSTANCE.getMouseButtonClicked(RIGHT) || IH_INSTANCE.isKeyPressed("escape"))
			&& SM_INSTANCE.isThereAReading()) {
			SM_INSTANCE.PopState();
		}
		if (IH_INSTANCE.isKeyPressed("f") ||
			((IH_INSTANCE.isKeyHeld("lalt") || IH_INSTANCE.isKeyHeld("ralt")) &&
				IH_INSTANCE.isKeyPressed("enter"))) {
			auto f = State::s_pDrawer->GetWindowFullscreen();
			State::s_pDrawer->SetWindowFullscreen(!f);
		}
	}
	
	void MenuState::Update() {
	}
	
	void MenuState::Render() {
	}

	void MenuState::Redraw(const MenuState& state) {
		WM_INSTANCE.WipeWidgets();
		m_ScreenToStart = state.m_ScreenToStart;
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