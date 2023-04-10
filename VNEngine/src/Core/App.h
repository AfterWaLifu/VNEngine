#pragma once

#include <string>
#include <vector>

#include "Core/Logger.h"
#include "Graphic/Artist.h"
#include "Audio/AudioPlayer.h"
#include "Controls/InputHandler.h"
#include "Widgets/Widget.h"

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

		std::vector<Widget*> m_Widgets;

		virtual void Load();

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