#include "ReadingState.h"
#include "vnepch.h"

#include "Audio/AudioPlayer.h"
#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/MenuState.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/InterfaceCreator.h"
#include "LuaReaders/StoryTeller.h"

namespace VNEngine {
	
	ReadingState::ReadingState(std::streampos startPos)
		:m_Loaded(false)
	{
		if (startPos != 0) {
			m_Loaded = true;
			SetReaderPos(startPos);
		}
	}

	void ReadingState::Handle() {
		if (IH_INSTANCE.getMouseButtonClicked(RIGHT)) {
			SM_INSTANCE.PushState(new MenuState("save"));
		}

		if (IH_INSTANCE.getMouseButtonClicked(LEFT) ||
			IH_INSTANCE.isKeyPressed("space") || IH_INSTANCE.isKeyPressed("enter")) {
			
			if (WM_INSTANCE.GetHiden()) {
				WM_INSTANCE.SetHiden(false);
			}
			else {
				m_ST.Go();
			}

			if (m_ST.GetCompleted()) {
				SM_INSTANCE.WipeStates();
				SM_INSTANCE.ChangeState(new MenuState("mainmenu"));
				return;
			}
		}

		if (IH_INSTANCE.isKeyHeld("lctrl") || IH_INSTANCE.isKeyHeld("rctrl")) m_ST.SetSkip(true);
		
		if (IH_INSTANCE.isKeyPressed("h")) {
			WM_INSTANCE.SetHiden(!(WM_INSTANCE.GetHiden()));
		}
	}

	void ReadingState::Update() {
	}

	void ReadingState::Render() {
	}

	bool ReadingState::onEnter() {
		if (!m_Loaded) {
			InterfaceCreator ic;
			ic.Draw("game");
		}
		m_ST.DoFile("game.lua");

		return true;
	}

	bool ReadingState::onExit() {
		WM_INSTANCE.WipeWidgets();
		AP_INSTANCE.StopMusic();
		s_pDrawer->WipeDrawing();
		return true;
	}

	std::string ReadingState::GetStateId() const {
		return m_ReadingState;
	}
	
	std::streampos ReadingState::GetReaderPos() {
		return m_ST.GetCurrentPos();
	}

	void ReadingState::SetReaderPos(std::streampos pos) {
		m_ST.SetCurrentPos(pos);
	}
}