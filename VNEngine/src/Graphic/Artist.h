#pragma once

#include <SDL2/SDL.h>

#include <string>

#include "Graphic/TextureManager.h"

namespace VNEngine{

	typedef SDL_Rect Rect;

	struct DrawnData {
		Texture texture;
		int tileNumber;
		Rect destination;
	};

	class Artist {
	private:
		SDL_Window* g_pWindow;
		SDL_Renderer* m_pRenderer;

		TextureManager* m_TextureManager;

		std::unordered_map<std::string, DrawnData> m_Queue;

	public:
		Artist(const std::string& title, int width, int height, bool fullscreen);
		~Artist();

		void Perform();

		void Draw(const std::string& key, int tileNum = 0, Rect destination = {0,0,100,100});
		void StopDrawing(const std::string& key);

		void AddTexture(const std::string& key, const std::string& path, int rows = 1, int collumns = 1);
		void DeleteTexture(const std::string& key);
	};
}