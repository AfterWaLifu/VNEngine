#include "TextureManager.h"
#include "vnepch.h"

#include <SDL2/SDL_image.h>

namespace VNEngine {

	TextureManager::TextureManager(SDL_Renderer* renderer)
		: m_Renderer(renderer)
	{
	}

	TextureManager::~TextureManager()
	{
		for (auto texture : m_Textures) SDL_DestroyTexture(texture.second);
		m_Textures.clear();
	}

	void TextureManager::addTexture(const std::string& key, const std::string& path)
	{
		SDL_Surface* tempSurface = IMG_Load(path.c_str());
		SDL_Texture* tempTexture = SDL_CreateTextureFromSurface(m_Renderer, tempSurface);
		SDL_FreeSurface(tempSurface);
		m_Textures[key] = tempTexture;
	}

	SDL_Texture* TextureManager::getTexture(const std::string& key)
	{
		return m_Textures[key];
	}

	void TextureManager::delTexture(const std::string& key)
	{
		SDL_DestroyTexture(m_Textures[key]);
		m_Textures.erase(key);
	}
}