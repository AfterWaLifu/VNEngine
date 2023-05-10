#pragma once

#include "StateMachine/State.h"

#include "Widgets/Text.h"
#include "Widgets/Button.h"
#include "Widgets/Textbox.h"

#include "LuaReaders/StoryTeller.h"

#include <fstream>

namespace VNEngine {

	class ReadingState : public State {

		std::string m_ReadingState = "reading";

		StoryTeller m_ST;

	public:
		void Handle() override;
		void Update() override;
		void Render() override;

		bool onEnter() override;
		bool onExit()  override;

		std::string GetStateId() const override;

		std::streampos GetReaderPos();
		void SetReaderPos(std::streampos pos);
	};
}