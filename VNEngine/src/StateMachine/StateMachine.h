#pragma once

#include "StateMachine/State.h"

#include <vector>

namespace VNEngine {

	class StateMachine {

		StateMachine();
		~StateMachine() = default;

		std::vector<State*> m_States;

		static inline StateMachine* s_StateMachine;

	public:
		static StateMachine& Instance();

		void PushState(State* state);
		void ChangeState(State* state);
		void PopState();

		void Update();
		void Draw();

		bool isThereAReading();
	};

#define SM_INSTANCE StateMachine::Instance()
}