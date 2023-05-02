#include "ReadingState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "Widgets/WidgetsManager.h"

namespace VNEngine {

	void ReadingState::Handle() {
	}

	void ReadingState::Update() {
	}

	void ReadingState::Render() {
	}

	bool ReadingState::onEnter() {
		m_ReadingState = "reading";

		return true;
	}

	bool ReadingState::onExit() {
		return true;
	}

	std::string ReadingState::GetStateId() const {
		return m_ReadingState;
	}
}