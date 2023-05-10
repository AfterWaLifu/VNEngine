#pragma once

#include "State.h"

namespace VNEngine {

	class MenuState : public State
	{
		std::string m_MenuState;
		std::string m_ScreenToStart;

		void handleSettings();
		void handleSave();
		void handleLoad();

	public:
		MenuState(const std::string& screen);

		void Handle() override;
		void Update() override;
		void Render() override;

		bool onEnter() override;
		bool onExit()  override;

		std::string GetStateId() const override;
	};
}

