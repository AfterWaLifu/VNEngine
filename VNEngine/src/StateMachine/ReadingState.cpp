#include "ReadingState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/InterfaceCreator.h"
#include "LuaReaders/StoryTeller.h"

namespace VNEngine {

	void ReadingState::Handle() {
		if (IH_INSTANCE.getMouseButtonState(LEFT)) {
			m_ST.Go();
		}
		if (IH_INSTANCE.isKeyHeld("lctrl")) m_ST.SetSkip(true);
		else m_ST.SetSkip(false);
	}

	void ReadingState::Update() {
	}

	void ReadingState::Render() {
	}

	bool ReadingState::onEnter() {
		m_ReadingState = "reading";
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