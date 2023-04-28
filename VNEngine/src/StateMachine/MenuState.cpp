#include "MenuState.h"
#include "vnepch.h"

namespace VNEngine {

	void MenuState::Handle() {

	}
	
	void MenuState::Update() {
	}
	
	void MenuState::Render() {
	}
	
	bool MenuState::onEnter() {
		return true;
	}

	bool MenuState::onExit() {
		return true;
	}

	std::string MenuState::GetStateId() const {
		return m_MenuState;
	}
}