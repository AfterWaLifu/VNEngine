#pragma once

#include <SDL2/SDL.h>

#include <string>
#include <stdint.h>

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

		std::unordered_map<uint32_t, DrawnData> m_Queue;
		uint32_t m_DrawId;

		void FindFirstEmptyId();

	public:
		Artist(const std::string& title, int width, int height, bool fullscreen);
		~Artist();

		void Perform();

		void Draw(const std::string& key, int tileNum = 0, Rect destination = {0,0,100,100});
		void Draw(const std::string& key, int row = 0, int collumn = 0, Rect destination = { 0,0,100,100 });
		void StopDrawing(const std::string& key);
		void StopDrawing(const uint32_t id);

		void AddTexture(const std::string& key, const std::string& path, int rows = 1, int collumns = 1);
		void DeleteTexture(const std::string& key);
	};
}