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

		int Update();
		int HandleEvents();
		int Draw();
		int m_WorkResult;

	public:
		App(AppInfo& defaultAppInfo);
		~App();

		int Run();
	};
}