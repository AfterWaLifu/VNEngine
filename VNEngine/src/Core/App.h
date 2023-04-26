#pragma once

#include <string>
#include <vector>

#include "Core/Logger.h"
#include "Graphic/Artist.h"
#include "Audio/AudioPlayer.h"
#include "Controls/InputHandler.h"
#include "Widgets/WidgetsManager.h"
#include "StateMachine/StateMachine.h"
#include "LuaReaders/Loader.h"

namespace VNEngine {

	/**
	* @brief Main app class
	* Being used to use/inherint and change smth if you need
	*/
	class App {
		friend Loader;
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
		App();
		virtual ~App();			/// App destructor

		void Run();				/// Function that starts work of the app
	};
}