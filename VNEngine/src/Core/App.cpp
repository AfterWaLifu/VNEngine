#include "App.h"
#include "vnepch.h"
#include "LuaReaders/InterfaceCreator.h"
namespace VNEngine {

	App::App()
		:m_Drawer(nullptr), m_IsRunning(false)
	{
		Loader loader = Loader(this);

		m_IsRunning = true;
	}

	App::~App() {
	}
	
	void App::Run() {
		Load();
		while (m_IsRunning) {
			Update();
			HandleEvents();
			Draw();
		}
	}

	void App::Load() { InterfaceCreator it; it.Draw("menu"); }

	void App::Update() {
		SDL_Delay(20);
	}

	void App::HandleEvents() {
		IH_INSTANCE.Update();
		WM_INSTANCE.Handle();
	}

	void App::Draw() {
		m_Drawer->Perform();
		WM_INSTANCE.Draw();
		m_Drawer->DrawAkaFinale();
	}
}