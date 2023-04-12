#pragma once

#include <SDL2/SDL.h>

#include <vector>
#include <string>

#include "Keys.h"
#include "Widgets/Vectors.h"

namespace VNEngine {

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

		bool getMouseButtonState(MouseButtons button);
		const vec2& getMousePos();
		bool isKeyDown(std::string key);
		bool getIfWindowResized();
		std::string* getTextInput();
		void setTextInput(bool state);
	};

#define IH_INSTANCE InputHandler::Instance()
}