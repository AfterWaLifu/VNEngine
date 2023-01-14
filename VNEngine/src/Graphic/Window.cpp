#include "Window.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	Window::Window(std::string title, int width, int height, bool fullscreen)
		: g_pWindow(nullptr), g_pRenderer(nullptr)
	{

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			logs.setMeta("ERROR");
			logs << "SDL initialization error";
			return;
		}

		int flags = 0;
		if (fullscreen) flags = SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN;
		else flags = SDL_WINDOW_SHOWN;

		g_pWindow = SDL_CreateWindow(
			title.c_str(),
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			width, height,
			flags
		);
		if (g_pWindow == nullptr) {
			logs.setMeta("ERROR");
			logs << "Window creation error";
			return;
		}

		g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);

		logs << "window creation succes";
	}

	Window::~Window() {
		SDL_DestroyRenderer(g_pRenderer);
		SDL_DestroyWindow(g_pWindow);
		SDL_Quit();

		logs << "window deleting succes";
	}

	void Window::Run(unsigned int ms) {
		SDL_SetRenderDrawColor(g_pRenderer, 200, 100, 100, 255);
		SDL_RenderClear(g_pRenderer);
		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(ms);
	}
}