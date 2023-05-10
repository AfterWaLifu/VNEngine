#pragma once

#include <unordered_map>
#include <string>

#include <SDL2/SDL.h>

namespace VNEngine {

	/**
	* @brief Texture data needed to draw it
	* Contains SDL texture, width+height and rows/collumns quantity
	*/
	struct Texture {
		SDL_Texture* sdl_texture;
		int w, h;
		int r, c;
		std::string key;
	};

	/**
	* @brief Manages textures, what else do you want from texture manager?
	* Contains textures with it's data and manages it (creation, containing, deleting)
	* It exist as singletone, to have an access from anywhere
	*/
	class TextureManager
	{
		std::unordered_map<std::string, Texture*> m_Textures;
		SDL_Renderer* m_Renderer;

	public:
		struct dumpT {
			std::string filename, key;
			int r, c;
		};
	private:
		std::vector<dumpT> m_ListOfPathes;
		void RemoveFromDumpList(const std::string& key);

		TextureManager(SDL_Renderer* renderer);
		~TextureManager();

		static inline TextureManager* s_pInstance;

		static inline std::string m_ImagesPath;
	public:

		bool addTexture(const std::string& key, const std::string& path, int rows, int collumns);
		Texture* getTexture(const std::string& key);
		bool delTexture(const std::string& key);

		static void TextureManagerInit(SDL_Renderer* renderer);
		static void TextureManagerTerminate();
		static TextureManager& Instance();

		static void SetImagesPath(const std::string& path);
		static std::string GetImagesPath();

		std::vector<dumpT> Dump();
	};

#define TM_INSTANCE TextureManager::Instance()
}
