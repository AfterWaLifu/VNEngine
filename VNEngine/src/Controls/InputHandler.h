#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <string>

#include "Keys.h"
#include "Widgets/Vectors.h"

namespace VNEngine {

	/**
	* @brief Class to handle all the input from keyboard or mouse in the app
	* Implemented as singletone to have an a access from anywhere.
	*/
	class InputHandler {
		
		static inline InputHandler* s_pInstance;

		bool* m_isRunningPointer;
		std::vector<bool> m_mouseButtonStates;
		vec2 m_mousePos;
		uint8_t* m_keystates;
		bool m_keyHolding;
		bool m_windowResized;
		bool m_textInputEnabled;
		std::string m_textInputText;

		void onKeyDown(SDL_Event& event);
		void onKeyUp();
		void onMouseMove(SDL_Event& event);
		void onMouseButtonUp(SDL_Event& event);
		void onMouseButtonDown(SDL_Event& event);
		void onWindowEvent(SDL_Event& event);
		void onTextInput(SDL_Event& event);
		
		InputHandler(bool* isRunnzing);
		~InputHandler();
	public:
		static InputHandler& Instance();
		static void InputHandlerInit(bool* isRunning);

		void Update();

		enum MouseButtons : uint8_t {
			LEFT = 0,
			MIDDLE = 1,
			RIGHT = 2
		};

		/**
		* @brief Function to get a state of mouse button
		* @param button A member of enum MouseButtons
		* @return True if preesed and false if not
		*/
		bool getMouseButtonState(MouseButtons button);

		/**
		* @brief Function to get a mouse cursor position
		* @return Vector of 2 coordinates x and y
		*/
		const vec2& getMousePos();

		/**
		* @brief Function to get a state of a keyboard button
		* @param key Button sign or name (check Keys.h)
		* @return True if pressed (and not holded*)
		*/
		bool isKeyDown(std::string key);
		
		/**
		* @brief Cheching for state of window size
		* @return True if window size changed
		*/
		bool getIfWindowResized();
		/**
		* @brief Function to set resized flag (needed for scaling)
		* @param key True if resized
		*/
		void setIfWindowResized(bool resized);

		/**
		* @brief Gives an access to the string of text input
		* @return Pointer to the string
		*/
		std::string* getTextInput();

		/**
		* @brief Turns on/off inputing text
		* @param state True or false to ON/OFF accordingly
		*/
		void setTextInput(bool state);
	};

#define IH_INSTANCE InputHandler::Instance()
}