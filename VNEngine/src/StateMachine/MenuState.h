#pragma once

#include "State.h"

#include "LuaReaders/InterfaceCreator.h"

namespace VNEngine {

	class MenuState : public State
	{
		std::string m_MenuState;
		std::string m_ScreenToStart;

		InterfaceCreator m_ic;

	public:
		MenuState(const std::string& screen);
		void Redraw(const std::string& screen);

		void Handle() override;
		void Update() override;
		void Render() override;

		bool onEnter() override;
		bool onExit()  override;

		std::string GetStateId() const override;
	};
}

