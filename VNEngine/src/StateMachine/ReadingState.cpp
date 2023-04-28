#include "ReadingState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "Widgets/WidgetsManager.h"

namespace VNEngine {

	void ReadingState::Handle() {
		if (IH_INSTANCE.getMouseButtonState(InputHandler::RIGHT) || IH_INSTANCE.isKeyPressed("esc")) {
			// todo		SM_INSTANCE.PushState( *MENU STATE* )
		}
		if (IH_INSTANCE.getMouseButtonState(InputHandler::LEFT)) {
			// todo		read next
		}
		if (IH_INSTANCE.isKeyPressed("lctrl") || IH_INSTANCE.isKeyPressed("rctrl")) {
			// todo		read skip
		}
		if (IH_INSTANCE.isKeyPressed("tab")) {
			//todo		togle skip
		}
		if (IH_INSTANCE.isKeyPressed("h")) {
			//todo		hide interface
		}
		if (IH_INSTANCE.isKeyPressed("s")) {
			//todo		screenshot
		}
		//todo			add mouse wheel up & down
	}

	void ReadingState::Update() {
	}

	void ReadingState::Render() {
	}

	bool ReadingState::onEnter() {

		return true;
	}

	bool ReadingState::onExit() {

		return true;
	}

	std::string ReadingState::GetStateId() const {
		return m_ReadingState;
	}
}