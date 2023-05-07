#include "ReadingState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/MenuState.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/InterfaceCreator.h"
#include "LuaReaders/StoryTeller.h"

namespace VNEngine {

	void ReadingState::Handle() {
		if (IH_INSTANCE.getMouseButtonState(RIGHT)) {
			SM_INSTANCE.PushState(new MenuState("save"));
		}

		if (IH_INSTANCE.getMouseButtonState(LEFT) ||
			IH_INSTANCE.isKeyPressed("space") || IH_INSTANCE.isKeyPressed("enter")) {
			
			if (WM_INSTANCE.GetHiden()) {
				WM_INSTANCE.SetHiden(false);
			}
			else {
				m_ST.Go();
			}
		}

		if (IH_INSTANCE.isKeyPressed("lctrl") || IH_INSTANCE.isKeyPressed("rctrl")) m_ST.SetSkip(true);
		
		if (IH_INSTANCE.isKeyPressed("h")) {
			WM_INSTANCE.SetHiden(!(WM_INSTANCE.GetHiden()));
		}
	}

	void ReadingState::Update() {
	}

	void ReadingState::Render() {
	}

	bool ReadingState::onEnter() {
		InterfaceCreator ic;
		ic.Draw("game");
		m_ST.DoFile("game.lua");

		return true;
	}

	bool ReadingState::onExit() {
		return true;
	}

	std::string ReadingState::GetStateId() const {
		return m_ReadingState;
	}
}