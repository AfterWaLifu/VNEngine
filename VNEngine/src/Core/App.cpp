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

		m_AudioPlayer->AddAudio("rickroll.wav", "r");
		m_AudioPlayer->PlayMusic("r");
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
		if (up) volume += 0.005f;
		else volume -= 0.005f;
		if (volume <= 0.0f) up = true;
		else if (volume >= 1.0f) up = false;
		m_AudioPlayer->SetMusicVolume(volume);
	}

	void App::HandleEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT) m_IsRunning = false;
	}

	void App::Draw() {
		m_Window->Run();
	}
}