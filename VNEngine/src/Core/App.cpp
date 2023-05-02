#include "App.h"
#include "vnepch.h"

namespace VNEngine {

	App::App()
		:m_Drawer(nullptr), m_IsRunning(false)
	{
		Loader loader = Loader(this);

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