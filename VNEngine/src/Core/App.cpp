#include "App.h"
#include "vnepch.h"

#include "Widgets/Text.h"

namespace VNEngine {

	App::App(AppInfo& defaultAppInfo)
		:m_Drawer(nullptr), m_IsRunning(false)
	{
		m_Drawer = std::make_unique<Artist>(
			defaultAppInfo.WindowTitle,
			defaultAppInfo.Width, defaultAppInfo.Height,
			defaultAppInfo.Fullscreen,
			&m_Widgets
		);

		m_AudioPlayer = std::make_unique<AudioPlayer>();

		m_InputHandler = std::make_unique<InputHandler>(&m_IsRunning);

		m_IsRunning = true;

		Load();
	}

	App::~App() {
		
	}
	
	void App::Run() {
		while (m_IsRunning) {
			Update();
			HandleEvents();
			Draw();
		}
	}

	void App::Load() { }

	void App::Update() {
		SDL_Delay(20);
	}

	void App::HandleEvents() {
		m_InputHandler->Update();
	}

	void App::Draw() {
		m_Drawer->Perform();
	}
}