#include "App.h"
#include "vnepch.h"

namespace VNEngine {

	App::App(AppInfo& defaultAppInfo)
		:m_Drawer(nullptr), m_IsRunning(false)
	{
		m_Drawer = std::make_unique<Artist>(
			defaultAppInfo.WindowTitle,
			defaultAppInfo.Width, defaultAppInfo.Height,
			defaultAppInfo.Fullscreen
		);

		m_AudioPlayer = std::make_unique<AudioPlayer>();

		InputHandler::InputHandlerInit(&m_IsRunning);

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
		IH_INSTANCE.Update();
	}

	void App::Draw() {
		m_Drawer->Perform();
		m_Drawer->DrawAkaFinale();
	}
}