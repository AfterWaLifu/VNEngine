#pragma once

#include <string>

#include "Core/Logger.h"
#include "Graphic/Window.h"

namespace VNEngine {

	struct AppInfo {
		std::string WindowTitle;
		int Width;
		int Height;
		bool Fullscreen;
	};

	class App {
		std::unique_ptr<Window> m_Window;

		void Update();
		void HandleEvents();
		void Draw();

		bool m_IsRunning;

	public:
		App(AppInfo& defaultAppInfo);
		~App();

		void Run();
	};
}