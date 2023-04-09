#include "App.h"
#include "vnepch.h"

#include "Widgets/Text.h"
#include "Widgets/Button.h"

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

		if (m_InputHandler->getMouseButtonState(InputHandler::MouseButtons::LEFT)) {
			for (auto w : m_Widgets) {
				if (!strcmp(typeid(*w).name(), "class VNEngine::Button")) {
					auto b = dynamic_cast<Button*>(w);
					b->Check(m_InputHandler->getMousePos());
				}
			}
		}
	}

	void App::Draw() {
		m_Drawer->Perform();
	}
}