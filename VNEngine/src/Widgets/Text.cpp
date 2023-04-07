#include "Text.h"
#include "vnepch.h"

#include <SDL2/SDL_ttf.h>

#include <stdlib.h>

#include "Core/Logger.h"

namespace VNEngine {

	Text::Text(vec4 geometry, std::wstring text, vec4u8 textColor, FontInfo fontInfo)
		: Widget()
	{
		m_Geometry = geometry;
		m_BackgroundColor = { 0,0,0,0 };
		m_Image = nullptr;

		m_TextTexture = nullptr;
		m_TextNativeGeometry = {0,0,0,0};
		m_TextDestination = { 0,0,0,0 };
		SetFont(fontInfo);
		m_TextColor = textColor;
		m_BackgroundTurned = true;

		if (!text.empty()) {
			SetText(text);
		}
	}

	Text::~Text() {
		freeTexture();
	}
	
	void Text::freeTexture() {
		if (m_TextTexture) {
			SDL_DestroyTexture(m_TextTexture);
			m_TextTexture = nullptr;
			m_Geometry = {};
		}
	}

	void Text::SetText(std::wstring text) {
		freeTexture();

		m_Text.clear();
		m_Text = text;

		SDL_Surface* textSurface = TTF_RenderUNICODE_Blended(
			m_Font.font, reinterpret_cast<Uint16 const*>(m_Text.c_str()),
			{m_TextColor.r,m_TextColor.g ,m_TextColor.b ,m_TextColor.a });

		if (!textSurface) {
			char* temp = (char*) malloc(m_Text.length());
			wcstombs_s(nullptr, temp, m_Text.length(), m_Text.c_str(), m_Text.length() * sizeof(wchar_t));
			VN_LOGS_WARNING("Can't create text label '" << temp << "'");
			return;
		}

		m_TextTexture = SDL_CreateTextureFromSurface(Widget::sRenderer, textSurface);
		if (!m_TextTexture) {
			char* temp = (char*)malloc(m_Text.length());
			wcstombs_s(nullptr, temp, m_Text.length(), m_Text.c_str(), m_Text.length() * sizeof(wchar_t));
			VN_LOGS_WARNING("Can't create text texture '" << temp << "'");
			return;
		}

		m_TextNativeGeometry = {0,0, textSurface->w, textSurface->h};

		SetAlign(m_Alignment);

		SDL_FreeSurface(textSurface);
	}

	std::wstring Text::GetText() {
		return m_Text;
	}

	void Text::SetFont(FontInfo fontInfo) {
		m_Font.info = fontInfo;
		m_Font.font = TTF_OpenFont(m_Font.info.fontName, m_Font.info.fontSize);
		if (!m_Font.font) {
			VN_LOGS_ERROR("Can't load font '" <<
				fontInfo.fontName << "', check other errors or fonts files location");
			return;
		}
		if (!m_Text.empty()) SetText(m_Text);
	}

	FontInfo Text::GetFont() {
		return m_Font.info;
	}

	void Text::Show() {
		m_IsShown = true;
	}

	void Text::Hide() {
		m_IsShown = false;
	}

	bool Text::IsItShown()
	{
		return m_IsShown;
	}

	void Text::SetTextColor(vec4u8 color) {
		m_TextColor = color;
	}

	vec4u8 Text::GetTextColor() {
		return m_TextColor;
	}

	void Text::TurnOnBack() {
		m_BackgroundTurned = true;
	}

	void Text::TurnOffBack() {
		m_BackgroundTurned = false;
	}

	void Text::SetBackgroundColor(vec4u8 color) {
		m_BackgroundColor = color;
	}

	vec4u8 Text::GetBackgroundColor() {
		return m_BackgroundColor;
	}

	void Text::SetAlign(Alignment alignment) {

		m_Alignment = alignment;

		m_TextDestination.y = m_Geometry.y + (m_Geometry.h - m_TextNativeGeometry.h+4) / 2;
		m_TextDestination.w = m_Geometry.w >= m_TextNativeGeometry.w ? m_TextNativeGeometry.w : m_Geometry.w;
		m_TextDestination.h = m_Geometry.h >= m_TextNativeGeometry.h ? m_TextNativeGeometry.h : m_Geometry.h;

		switch (m_Alignment) {
		case ALIGN_LEFT:
			m_TextDestination.x = m_Geometry.x;
			break;
		case ALIGN_RIGHT:
			m_TextDestination.x = m_Geometry.x + m_Geometry.w-m_TextNativeGeometry.w;
			break;
		case ALIGN_CENTER:
			m_TextDestination.x = m_Geometry.x + (m_Geometry.w - m_TextNativeGeometry.w) / 2;
			break;
		}
	}

	Alignment Text::GetAlign() {
		return m_Alignment;
	}

	void Text::SetBackImage(std::string key) {
		m_Image = TM_INSTANCE.getTexture(key);
	}

	void Text::Draw() {
		if (!m_IsShown) return;

		if (m_Image) {
			SDL_RenderCopy(Widget::sRenderer, m_Image->sdl_texture,
				nullptr, (SDL_Rect*)&m_Geometry);
		}
		else if (m_BackgroundTurned && m_BackgroundColor.a != 0) {
			SDL_SetRenderDrawColor(Widget::sRenderer, m_BackgroundColor.r,
				m_BackgroundColor.g, m_BackgroundColor.b, m_BackgroundColor.a);
			SDL_RenderFillRect(Widget::sRenderer, (SDL_Rect*)&m_Geometry);
		}

		SDL_RenderCopy(Widget::sRenderer, m_TextTexture,
			(SDL_Rect*)&m_TextNativeGeometry, (SDL_Rect*)&m_TextDestination);
	}

}