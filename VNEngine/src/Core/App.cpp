#include "App.h"
#include "vnepch.h"

namespace VNEngine {

	App::App()
		:m_Drawer(nullptr), m_IsRunning(false)
	{
		Loader l= Loader(this);
		l.Load();

		m_IsRunning = true;

		Load();
	}

	App::~App() {
		Loader l(this);
		l.Save();
	}
	
	void App::Run() {
		uint64_t TICKS, diff;
		while (m_IsRunning) {
			TICKS = SDL_GetTicks64();
			Update();
			HandleEvents();
			Draw();
			diff = TICKS = SDL_GetTicks64();
			if (diff < 16) SDL_Delay((uint32_t)(16 - diff));
		}
	}

	void App::Load() {
		SM_INSTANCE.ChangeState(new MenuState("mainmenu"));
	}

	void App::Update() {
	}

	void App::HandleEvents() {
		IH_INSTANCE.Update();
		WM_INSTANCE.Handle();
		SM_INSTANCE.Update();
	}

	void App::Draw() {
		m_Drawer->Perform();
		SM_INSTANCE.Draw();
		WM_INSTANCE.Draw();
		m_Drawer->DrawAkaFinale();
	}
}