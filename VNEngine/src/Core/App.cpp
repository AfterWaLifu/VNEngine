#include "App.h"
#include "vnepch.h"

namespace VNEngine {

	App::App(AppInfo& defaultAppInfo)
		:m_Window(nullptr), m_IsRunning(false)
	{
		
		m_Window = std::make_unique<Window>(
			defaultAppInfo.WindowTitle,
			defaultAppInfo.Width, defaultAppInfo.Height,
			defaultAppInfo.Fullscreen
		);

		m_AudioPlayer = std::make_unique<AudioPlayer>();

		m_IsRunning = true;
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

	void App::Update() {
		SDL_Delay(20);
	}

	void App::HandleEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT) m_IsRunning = false;
	}

	void App::Draw() {

	}
}