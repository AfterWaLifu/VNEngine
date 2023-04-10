#pragma once

#include <SDL2/SDL.h>

#include <vector>

#include "Keys.h"
#include "Widgets/Vectors.h"

namespace VNEngine {

	class InputHandler {
		
		static inline InputHandler* s_pInstance;

		bool* m_isRunningPointer;
		std::vector<bool> m_mouseButtonStates;
		vec2 mousePos;
		uint8_t* m_keystates;
		bool windowResized;

		void onKeyDown();
		void onMouseMove(SDL_Event& event);
		void onMouseButtonUp(SDL_Event& event);
		void onMouseButtonDown(SDL_Event& event);
		void onWindowEvent(SDL_Event& event);
		
		InputHandler(bool* isRunning);
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
		bool isKeyDown(SDL_Scancode key);
		bool getIfWindowResized();
	};

#define IH_INSTANCE InputHandler::Instance()
}