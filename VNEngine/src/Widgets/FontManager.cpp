#include "FontManager.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {
	
	FontManager::FontManager() {
	}

	FontManager::~FontManager() {
		for (auto f : m_Fonts) TTF_CloseFont(f.second.font);
	}

	void FontManager::addFont(const std::string& key, const std::string& filename, int size) {
		TTF_Font* f = TTF_OpenFont((sFontPath + filename).c_str(), size);
		if (f == nullptr) {
			VN_LOGS_WARNING("Cant open font " << sFontPath + filename);
			return;
		}
		
		if (m_Fonts.find(key) == m_Fonts.end()) {
			Font fontToInsert = { filename, size, f };
			m_Fonts.insert( std::make_pair(key, fontToInsert));
		}
		else {
			VN_LOGS_WARNING("Font w/ a key below already exist" << key);
		}
	}

	Font* FontManager::getFont(const std::string& key) {
		if (m_Fonts.find(key) == m_Fonts.end()) {
			VN_LOGS_WARNING("Attemp to get a non-existing font");
			return nullptr;
		}
		return &(m_Fonts[key]);
	}

	void FontManager::delFont(const std::string& key) {
		if (m_Fonts.find(key) == m_Fonts.end()) {
			VN_LOGS_WARNING("Attemp to delete a non-existing font");
			return;
		}
		TTF_CloseFont(m_Fonts[key].font);
		m_Fonts.erase(key);
	}

	FontManager& FontManager::Instance() {
		return s_pInstance ? *s_pInstance : *(s_pInstance = new FontManager());
	}

	void FontManager::SetFontPath(const std::string& path) {
		sFontPath = path;
	}

	std::string FontManager::GetFontPath() {
		return sFontPath;
	}

	void FontManager::SetDefaultFont(const std::string& font) {
		sDefaultFont = font;
	}
	
	std::string FontManager::GetDefaultFont() {
		return sDefaultFont;
	}
}