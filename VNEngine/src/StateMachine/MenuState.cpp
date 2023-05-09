#include "MenuState.h"
#include "vnepch.h"

#include "Controls/InputHandler.h"
#include "StateMachine/StateMachine.h"
#include "StateMachine/ReadingState.h"
#include "Widgets/WidgetsManager.h"
#include "LuaReaders/InterfaceCreator.h"
#include "Audio/AudioPlayer.h"

namespace VNEngine {
	
	MenuState::MenuState(const std::string& screen) : m_ScreenToStart(screen), m_MenuState(screen) {
	}
	
	void MenuState::Handle() {
		if (WM_INSTANCE.ExistsButton("start")) {
			if (WM_INSTANCE.GetButton("start")->Pressed()) {
				SM_INSTANCE.ChangeState(new ReadingState());
			}
		}
		if (WM_INSTANCE.ExistsButton("save")) {
			if (WM_INSTANCE.GetButton("save")->Pressed()) {
				WM_INSTANCE.WipeWidgets();
				InterfaceCreator ic;
				m_ScreenToStart = "save";
				ic.Draw(m_ScreenToStart);
			}
		}
		if (WM_INSTANCE.ExistsButton("load")) {
			if (WM_INSTANCE.GetButton("load")->Pressed()) {
				InterfaceCreator ic;
				m_ScreenToStart = "load";
				ic.Draw(m_ScreenToStart);
			}
		}
		if (WM_INSTANCE.ExistsButton("settings")) {
			if (WM_INSTANCE.GetButton("settings")->Pressed()) {
				InterfaceCreator ic;
				m_ScreenToStart = "settings";
				ic.Draw(m_ScreenToStart);
			}
		}
		if (WM_INSTANCE.ExistsButton("mainmenu")) {
			if (WM_INSTANCE.GetButton("mainmenu")->Pressed()) {
				SM_INSTANCE.WipeStates();
				SM_INSTANCE.ChangeState(new MenuState("mainmenu"));
			}
		}
		if (WM_INSTANCE.ExistsButton("exit")) {
			if (WM_INSTANCE.GetButton("exit")->Pressed()) {
				IH_INSTANCE.exit();
			}
		}

		handleSettings();

		if (IH_INSTANCE.getMouseButtonState(RIGHT) && SM_INSTANCE.isThereAReading()) {
			SM_INSTANCE.PopState();
		}
	}

	void MenuState::handleSettings() {
		if (WM_INSTANCE.ExistsButton("music-")) {
			if (WM_INSTANCE.GetButton("music-")->Pressed()) {
				float v = AP_INSTANCE.GetMusicVolume();
				v -= 0.05f;
				if (v <= 0.0f) v = 0.0f;
				AP_INSTANCE.SetMusicVolume( v );
				WM_INSTANCE.GetText("musicVolume")->SetText(std::to_string(v).substr(0,4));
			}
		}
		if (WM_INSTANCE.ExistsButton("music+")) {
			if (WM_INSTANCE.GetButton("music+")->Pressed()) {
				float v = AP_INSTANCE.GetMusicVolume();
				v += 0.05f;
				if (v >= 1.0f) v = 1.0f;
				AP_INSTANCE.SetMusicVolume(v);
				WM_INSTANCE.GetText("musicVolume")->SetText(std::to_string(v).substr(0, 4));
			}
		}
		if (WM_INSTANCE.ExistsButton("sound-")) {
			if (WM_INSTANCE.GetButton("sound-")->Pressed()) {
				float v = AP_INSTANCE.GetSoundVolume();
				v -= 0.05f;
				if (v <= 0.0f) v = 0.0f;
				AP_INSTANCE.SetSoundVolume(v);
				WM_INSTANCE.GetText("soundVolume")->SetText(std::to_string(v).substr(0, 4));
			}
		}
		if (WM_INSTANCE.ExistsButton("sound+")) {
			if (WM_INSTANCE.GetButton("sound+")->Pressed()) {
				float v = AP_INSTANCE.GetSoundVolume();
				v += 0.05f;
				if (v >= 1.0f) v = 1.0f;
				AP_INSTANCE.SetSoundVolume(v);
				WM_INSTANCE.GetText("soundVolume")->SetText(std::to_string(v).substr(0, 4));
			}
		}
		if (WM_INSTANCE.ExistsButton("muteButton")) {
			if (WM_INSTANCE.GetButton("muteButton")->Pressed()) {
				bool muted = AP_INSTANCE.GetIfMute();
				if (muted) {
					AP_INSTANCE.Unmute();
					WM_INSTANCE.GetButton("muteButton")->SetText(L"Mute");
				}
				else {
					AP_INSTANCE.Mute();
					WM_INSTANCE.GetButton("muteButton")->SetText(L"Unmute");
				}
			}
		}
	}
	
	void MenuState::Update() {
	}
	
	void MenuState::Render() {
	}
	
	bool MenuState::onEnter() {
		InterfaceCreator ic;
		ic.Draw(m_ScreenToStart);

		return true;
	}

	bool MenuState::onExit() {
		WM_INSTANCE.WipeWidgets();
		AP_INSTANCE.StopMusic();
		s_pDrawer->WipeDrawing();
		return true;
	}

	std::string MenuState::GetStateId() const {
		return m_MenuState;
	}
}