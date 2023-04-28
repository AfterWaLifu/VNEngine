#pragma once

#include "State.h"

namespace VNEngine {

	class MenuState : public State
	{
		std::string m_MenuState;

	public:
		void Handle() override;
		void Update() override;
		void Render() override;

		bool onEnter() override;
		bool onExit()  override;

		std::string GetStateId() const override;
	};
}

