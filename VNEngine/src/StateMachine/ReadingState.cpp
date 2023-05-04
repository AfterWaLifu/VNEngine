#include "ReadingState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/StoryTeller.h"

namespace VNEngine {

	void ReadingState::Handle() {
	}

	void ReadingState::Update() {
	}

	void ReadingState::Render() {
	}

	bool ReadingState::onEnter() {
		m_ReadingState = "reading";
		StoryTeller st;
		st.DoFile("game.lua");

		return true;
	}

	bool ReadingState::onExit() {
		return true;
	}

	std::string ReadingState::GetStateId() const {
		return m_ReadingState;
	}
}