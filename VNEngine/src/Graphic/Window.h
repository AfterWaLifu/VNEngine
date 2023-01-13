#pragma once

#include <SDL2/SDL.h>

namespace VNEngine{

	class Window {
	private:
		SDL_Window* g_pWindow;
		SDL_Renderer* g_pRenderer;

	public:
		Window();
		~Window();

		void Run(unsigned int ms);
	};
}