#include "App.h"
#include "vnepch.h"

#include "Widgets/Text.h"
#include "Widgets/Button.h"
#include "Widgets/TextBox.h"

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

		for (auto w : m_Widgets) {
			TextBox* tb = (TextBox*)w;
			tb->Check();
		}
	}

	void App::Draw() {
		m_Drawer->Perform();
	}
}