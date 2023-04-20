#pragma once

#include "StateMachine/State.h"

#include <vector>

namespace VNEngine {

	class StateMachine {

		std::vector<State*> m_States;

		static inline StateMachine* s_StateMachine;

	public:
		static StateMachine& Instance();

		void PushState(State* state);
		void ChangeState(State* state);
		void PopState();
	};

#define SM_INSTANCE StateMachine::Instance()
}