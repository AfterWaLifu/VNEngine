#include "Artist.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	Rect& GetTileRect(const Texture& t, const int tile) {
		Rect source = { 0,0,1,1 };
		if (t.r * t.c > tile) {
			source.w = t.w / t.c;
			source.h = t.h / t.r;
			source.x = source.w * (tile%t.c);
			source.y = source.h * (tile/t.c);
		}
		return source;
	}
	Rect& GetTileRect(const Texture& t, const int row, const int collumn) {
		Rect source = { 0,0,1,1 };
		if (t.r > row || t.c > collumn) {
			source.w = t.w / t.c;
			source.h = t.h / t.r;
			source.x = source.w * collumn;
			source.y = source.h * row;
		}
		return source;
	}

	Artist::Artist(const std::string& title, int width, int height, bool fullscreen)
		: g_pWindow(nullptr), m_pRenderer(nullptr), m_TextureManager(nullptr)
	{

		if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
			VN_LOGS_ERROR("SDL initialization error");
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
			VN_LOGS_ERROR("Window creation error");
			return;
		}

		m_pRenderer = SDL_CreateRenderer(g_pWindow, -1, 0);
		
		m_TextureManager = new TextureManager(m_pRenderer);
		m_Queue.clear();

		VN_LOGS_INFO("Window creation succes");
	}

	Artist::~Artist() {
		m_Queue.clear();
		delete m_TextureManager;
		SDL_DestroyRenderer(m_pRenderer);
		SDL_DestroyWindow(g_pWindow);
		SDL_Quit();

		VN_LOGS_INFO("Window deleting succes");
	}

	void Artist::Perform() {
		SDL_SetRenderDrawColor(m_pRenderer, 200, 100, 100, 255);
		SDL_RenderClear(m_pRenderer);
		
		for (auto pair : m_Queue) {
			//todo
		}

		SDL_RenderPresent(m_pRenderer);
	}

	void Artist::Draw(const std::string& key, int tileNum, Rect destination) {
		//todo
	}

	void Artist::StopDrawing(const std::string& key) {
		m_Queue.erase(key);
	}

	void Artist::AddTexture(const std::string& key, const std::string& path, int rows, int collumns) {
		m_TextureManager->addTexture(key, path, rows, collumns);
	}

	void Artist::DeleteTexture(const std::string& key) {
		m_TextureManager->delTexture(key);
	}

}