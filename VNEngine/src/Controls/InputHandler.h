#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <string>
#include <stdint.h>

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
		std::vector<bool> m_mouseButtonHold;
		vec2 m_mousePos;
		int32_t m_mouseScrollAmount;
		uint8_t* m_keystates;
		std::vector<SDL_Scancode> m_keysPressed;
		std::vector<SDL_Scancode> m_keysHold;
		bool m_windowResized;
		bool m_textInputEnabled;
		std::string m_textInputText;

		void onKeyDown(SDL_Event& event);
		void onKeyUp(SDL_Event& event);
		void onMouseMove(SDL_Event& event);
		void onMouseButtonUp(SDL_Event& event);
		void onMouseButtonDown(SDL_Event& event);
		void onMouseWheelEvent(SDL_Event& event);
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
		* @brief Mouse scroll event use
		* @return Amount of pixels scrolled (positive=up, negative=down)
		*/
		int32_t getMouseScrollAmount();

		/**
		* @brief Function to get a pressed of a keyboard button
		* @param key Button sign or name (check Keys.h)
		* @return True if pressed (and not holded)
		*/
		bool isKeyPressed(const std::string& key);

		/*
		* @brief Function to get a hold state of keyboard button
		* @param key Button sign or name (check Keys.h)
		* @return True if being held
		*/
		bool isKeyHeld(const std::string& key);

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
		void setTextInputState(bool state);

		bool getTextInputState();
	};

#define IH_INSTANCE InputHandler::Instance()
}