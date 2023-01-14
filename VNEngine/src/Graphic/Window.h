#pragma once

#include <SDL2/SDL.h>

#include <string>

namespace VNEngine{

	class Window {
	private:
		SDL_Window* g_pWindow;
		SDL_Renderer* g_pRenderer;

	public:
		Window(std::string title, int width, int height, bool fullscreen);
		~Window();

		void Run(unsigned int ms);
	};
}