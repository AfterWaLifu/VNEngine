#pragma once

#include <SDL2/SDL_ttf.h>

#include <unordered_map>
#include <string>

namespace VNEngine {

	struct Font {
		std::string fontName;
		int fontSize;
		TTF_Font* font;
	};

	class FontManager {

		std::unordered_map<std::string, Font> m_Fonts;

		FontManager();
		~FontManager();

		static inline FontManager* s_pInstance;

		static inline std::string sFontPath;
		static inline std::string sDefaultFont;
		
	public:

		void addFont(const std::string& key, const std::string& filename, int size);
		Font* getFont(const std::string& key);
		void delFont(const std::string& key);

		static FontManager& Instance();

		static void SetFontPath(const std::string& path);
		static std::string GetFontPath();
		static void SetDefaultFont(const std::string& font);
		static std::string GetDefaultFont();
	};
#define FM_INSTANCE FontManager::Instance()
}