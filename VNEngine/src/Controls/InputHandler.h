#pragma once

#include <SDL2/SDL.h>

#include <vector>

namespace VNEngine {

	struct vec2 {
		int x, y;
	};

	class InputHandler {
		
		bool isRunning;
		std::vector<bool> m_mouseButtonStates;
		vec2 mousePos;
		uint8_t* m_keystates;
		bool windowResized;

		void onKeyDown();
		void onMouseMove(SDL_Event& event);
		void onMouseButtonUp(SDL_Event& event);
		void onMouseButtonDown(SDL_Event& event);
		void onWindowEvent(SDL_Event& event);

	public:
		InputHandler();
		~InputHandler();

		void Update();

		enum MouseButtons : uint8_t {
			LEFT = 0,
			MIDDLE = 1,
			RIGHT = 2
		};

		bool getIfRunning();
		bool getMouseButtonState(MouseButtons button);
		const vec2& getMousePos();
		bool isKeyDown(SDL_Scancode key);
		bool getIfWindowResized();
	};
}