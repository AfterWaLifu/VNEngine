#pragma once

#include "StateMachine/State.h"

#include <vector>
#include <fstream>

struct lua_State;

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
		void WipeStates();

		void Update();
		void Draw();

		bool isThereAReading();

		std::streampos GetTopReaderPos();
		void SetTopReaderPos(size_t pos);
		lua_State* GetTopReaderState();

		std::string GetTopStateID();
		Artist* GetCurrentDrawer();
	};

#define SM_INSTANCE StateMachine::Instance()
}