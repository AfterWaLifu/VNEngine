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

	void App::Load()
	{
		m_Drawer->AddTexture("m", "masyunya.png");
		m_Drawer->AddTexture("n", "neverhood.png");
	}

	static int a = 0;
	static char texture[] = "m";
	void App::Update() {
		SDL_Delay(20);
		++a;
		if (a == 50) {
			if (texture[0] == 'm') {
				m_Drawer->StopDrawing("m");
				m_Drawer->Draw("n");
				texture[0] = 'n';
			}
			else {
				m_Drawer->StopDrawing("n");
				m_Drawer->Draw("m");
				texture[0] = 'm';
			}
			a = 0;
		}
	}

	void App::HandleEvents() {
		SDL_Event event;
		SDL_PollEvent(&event);

		if (event.type == SDL_QUIT) m_IsRunning = false;
	}

	void App::Draw() {
		m_Drawer->Perform();
	}
}