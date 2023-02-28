#pragma once

#include <SDL2/SDL.h>

#include <string>

#include "Graphic/TextureManager.h"

namespace VNEngine{

	class Artist {
	private:
		SDL_Window* g_pWindow;
		SDL_Renderer* m_pRenderer;

		TextureManager* m_TextureManager;

		std::unordered_map<std::string, SDL_Texture*> m_Queue;

	public:
		Artist(const std::string& title, int width, int height, bool fullscreen);
		~Artist();

		void Perform();

		void Draw(const std::string& key);
		void StopDrawing(const std::string& key);

		void AddTexture(const std::string& key, const std::string& path);
		void DeleteTexture(const std::string& key);
	};
}