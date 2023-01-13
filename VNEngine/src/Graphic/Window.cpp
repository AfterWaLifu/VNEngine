#include "Window.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	Window::Window() 
		: g_pWindow(nullptr), g_pRenderer(nullptr)
	{

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			logs.setMeta("ERROR");
			logs << "SDL initialization error";
			return;
		}

		g_pWindow = SDL_CreateWindow(
			"Cool Title", 
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			1280, 720,
			SDL_WINDOW_SHOWN
		);
		if (g_pWindow == nullptr) {
			logs.setMeta("ERROR");
			logs << "Window creation error";
			return;
		}

		g_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);

	}

	Window::~Window() {
		SDL_DestroyRenderer(g_pRenderer);
		SDL_DestroyWindow(g_pWindow);
		SDL_Quit();
	}

	void Window::Run(unsigned int ms) {
		SDL_SetRenderDrawColor(g_pRenderer,200,100,100, 255);
		SDL_RenderClear(g_pRenderer);
		SDL_RenderPresent(g_pRenderer);
		SDL_Delay(ms);
	}
}