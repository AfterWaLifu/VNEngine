#pragma once

#include <string>
#include <vector>

#include "Core/Logger.h"
#include "Graphic/Artist.h"
#include "Audio/AudioPlayer.h"
#include "Controls/InputHandler.h"
#include "Widgets/WidgetsManager.h"
#include "StateMachine/StateMachine.h"

namespace VNEngine {

	/**
	* @brief Info needed to create a window you need
	* Contains title string, width, height and window-fullscreen state
	*/
	struct AppInfo {
		std::string WindowTitle;
		int Width;
		int Height;
		bool Fullscreen;
	};

	/**
	* @brief Main app class
	* Being used to use/inherint and change smth if you need
	*/
	class App {
	protected:
		std::unique_ptr<Artist> m_Drawer;	/// Artist class object, works with graphics

		virtual void Load();	/// Function for preparing for work

		virtual void Update();	/// Func to change smth you need and when you need
		void HandleEvents();	/// Handles events (keys, mouse etc)
		void Draw();			/// Calling drawing of everything in order

		bool m_IsRunning;		/// State of the app, if it should be closed (autochanged)

	public:
		/**
		* @brief App constructor, creates App
		* @param	defaultAppInfo	Window default params
		*/
		App(AppInfo& defaultAppInfo);
		virtual ~App();			/// App destructor

		void Run();				/// Function that starts work of the app
	};
}