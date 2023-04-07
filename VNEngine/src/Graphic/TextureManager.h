#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

namespace VNEngine {

	struct Texture {
		SDL_Texture* sdl_texture;
		int w, h;
		int r, c;
	};

	class TextureManager
	{
		std::unordered_map<std::string, Texture*> m_Textures;
		SDL_Renderer* m_Renderer;

		TextureManager(SDL_Renderer* renderer);
		~TextureManager();

		static inline TextureManager* s_pInstance;
	public:

		bool addTexture(const std::string& key, const std::string& path, int rows, int collumns);
		Texture* getTexture(const std::string& key);
		bool delTexture(const std::string& key);

		static void TextureManagerInit(SDL_Renderer* renderer);
		static void TextureManagerTerminate();
		static TextureManager& Instance();
	};

#define TM_INSTANCE TextureManager::Instance()
}
