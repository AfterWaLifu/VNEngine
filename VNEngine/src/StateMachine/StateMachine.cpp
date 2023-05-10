#include "StateMachine.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "StateMachine/MenuState.h"
#include "StateMachine/ReadingState.h"
#include "Widgets/WidgetsManager.h"
#include "Audio/AudioPlayer.h"

namespace VNEngine {

	StateMachine::StateMachine() {
		m_States = std::vector<State*>();
	}

	StateMachine& StateMachine::Instance() {
		return (s_StateMachine) ? *s_StateMachine : *(s_StateMachine = new StateMachine()) ;
	}

	void StateMachine::PushState(State* state) {
		if (!m_States.empty()) {
			WM_INSTANCE.SaveScreen();
			State::s_pDrawer->SaveScreen();
			AP_INSTANCE.PushState();
		}

		m_States.push_back(state);
		if (!(m_States.back()->onEnter())) {
			VN_LOGS_WARNING("Something wrong on entering " << m_States.back()->GetStateId() << " state");
		}
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

		if (!m_States.empty()) {
			WM_INSTANCE.PopScreen();
			State::s_pDrawer->PopScreen();
			AP_INSTANCE.PopState();
		}
	}

	void StateMachine::WipeStates() {
		while (!m_States.empty()) PopState();
		WM_INSTANCE.WipeScreens();
		State::s_pDrawer->WipeScreens();
		AP_INSTANCE.WipeStates();
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

	bool StateMachine::isThereAReading() {
		if (m_States.empty()) return false;
		for (auto s : m_States) {
			if (s->GetStateId() == "reading") return true;
		}
		return false;
	}
	
	std::streampos StateMachine::GetTopReaderPos() {
		auto back = m_States.end();
		std::streampos sp(0);
		while (back != m_States.begin() || (*back)->GetStateId() != "reading") {
			--back;
		}
		if ((*back)->GetStateId() == "reading") {
			ReadingState* r = dynamic_cast<ReadingState*>(*back);
			sp = r->GetReaderPos();
		}
		return sp;
	}
}