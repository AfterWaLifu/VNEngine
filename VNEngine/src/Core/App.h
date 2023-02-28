#pragma once

#include <string>

#include "Core/Logger.h"
#include "Graphic/Artist.h"
#include "Audio/AudioPlayer.h"

namespace VNEngine {

	struct AppInfo {
		std::string WindowTitle;
		int Width;
		int Height;
		bool Fullscreen;
	};

	class App {
	protected:
		std::unique_ptr<Artist> m_Drawer;
		std::unique_ptr<AudioPlayer> m_AudioPlayer;

		void Update();
		void HandleEvents();
		void Draw();

		bool m_IsRunning;

	public:
		App(AppInfo& defaultAppInfo);
		virtual ~App();

		void Run();
	};
}