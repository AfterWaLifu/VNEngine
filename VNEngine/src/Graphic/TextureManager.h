#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

namespace VNEngine {

	class TextureManager
	{
		std::unordered_map<std::string, SDL_Texture*> m_Textures;
		SDL_Renderer* m_Renderer;

	public:
		TextureManager(SDL_Renderer* renderer);
		~TextureManager();

		bool addTexture(const std::string& key, const std::string& path);
		SDL_Texture* getTexture(const std::string& key);
		bool delTexture(const std::string& key);
	};
}
