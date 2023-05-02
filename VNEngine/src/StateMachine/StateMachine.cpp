#include "StateMachine.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	StateMachine::StateMachine() {
		m_States = std::vector<State*>();
	}

	StateMachine& StateMachine::Instance() {
		return (s_StateMachine) ? *s_StateMachine : *(s_StateMachine = new StateMachine()) ;
	}

	void StateMachine::PushState(State* state) {
		m_States.push_back(state);
		if ( !(m_States.back()->onEnter()) ) 
			VN_LOGS_WARNING("Something wrong on entering " << m_States.back()->GetStateId() << " state");
	}

	void StateMachine::ChangeState(State* state) {
		if (!m_States.empty()) {

			if (m_States.back()->GetStateId() == state->GetStateId()) {
				return;
			}

			if (m_States.back()->onExit()) {
				delete m_States.back();
				m_States.pop_back();
			}
			else {
				VN_LOGS_WARNING("Something wrong on exiting " << m_States.back()->GetStateId() << " state");
			}
		}

		PushState(state);
	}

	void StateMachine::PopState() {
		if (!m_States.empty()) {
			if (!(m_States.back()->onExit())) {
				VN_LOGS_WARNING("Something wrong on exiting " << m_States.back()->GetStateId() << " state");
			} else {
				delete m_States.back();
				m_States.pop_back();
			}
		}
		else VN_LOGS_WARNING("Attemp to pop state of empty stack");
	}
	
	void StateMachine::Update() {
		if (!(m_States.empty())) {
			m_States.back()->Handle();
			m_States.back()->Update();
		}
	}

	void StateMachine::Draw() {
		if (!(m_States.empty())) {
			m_States.back()->Render();
		}
	}
}