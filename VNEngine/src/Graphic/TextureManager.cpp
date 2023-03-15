#include "TextureManager.h"
#include "vnepch.h"

#include <SDL2/SDL_image.h>

#include "Core/Logger.h"

namespace VNEngine {

	TextureManager::TextureManager(SDL_Renderer* renderer)
		: m_Renderer(renderer)
	{
		int flags = IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_WEBP | IMG_INIT_TIF;
		int init = IMG_Init(flags);
		if ((flags & init) != flags) {
			VN_LOGS_WARNING("Failed to init required image formats");
			VN_LOGS_ERROR(IMG_GetError());
		}
	}

	TextureManager::~TextureManager()
	{
		IMG_Quit();
		for (auto texture : m_Textures) SDL_DestroyTexture(texture.second->sdl_texture);
		m_Textures.clear();
	}

	bool TextureManager::addTexture(const std::string& key, const std::string& path, int rows, int collumns)
	{
		SDL_Surface* tempSurface = IMG_Load(path.c_str());
		if (tempSurface == nullptr) {
			VN_LOGS_WARNING("Image file not found '" << path << "', check error below");
			VN_LOGS_WARNING(IMG_GetError());
			return false;
		}
		SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(m_Renderer, tempSurface);
		SDL_FreeSurface(tempSurface);

		if (tempTexture) {
			int w = 0, h = 0;
			SDL_QueryTexture(tempTexture, nullptr, nullptr, &w, &h);

			if (w == 0 || h == 0) {
				VN_LOGS_WARNING("Something went wrong on getting width&height of texture");
				SDL_DestroyTexture(tempTexture);
				return false;
			}

			m_Textures[key] = new Texture({tempTexture, w, h, rows, collumns});
			return true;
		}
		VN_LOGS_WARNING("Something went wrong on creating texture '" << key << "'");
		return false;
	}

	Texture* TextureManager::getTexture(const std::string& key)
	{
		if (m_Textures[key]) return m_Textures[key];
		else {
			VN_LOGS_WARNING("Attemp to get non-existing texture with key '" << key << "'");
			return nullptr;
		}
	}

	bool TextureManager::delTexture(const std::string& key)
	{
		SDL_DestroyTexture(m_Textures[key]->sdl_texture);
		m_Textures.erase(key);
		if (SDL_GetError()) {
			VN_LOGS_WARNING("Something went wrong on deleting texture '" << key << "'");
		}
		return true;
	}
}