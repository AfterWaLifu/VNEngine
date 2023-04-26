#pragma once

#include "Widgets/Widget.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace VNEngine {

	struct FontInfo {
		std::string fontName;
		int fontSize;
	};

	class Text : public Widget {

	protected:
		SDL_Texture* m_TextTexture;
		struct {
			FontInfo info;
			TTF_Font* font;
		} m_Font;
		
		vec4 m_TextNativeGeometry;
		vec4 m_TextDestination;
		int m_IndentVertical, m_IndentHorizontal;
		bool m_Wraped;

		vec4u8 m_TextColor;
		std::wstring m_Text;

		vec4u8 m_BorderColor;
		bool m_DrawBorder;

		void freeTexture();
		void windowResized();

	public:
		Text(vec4 geometry, std::wstring text = L"", vec4u8 textColor = {0,0,0,255},
			FontInfo fontInfo = { "", 16 });
		virtual ~Text();

		void SetText(std::wstring text);
		std::wstring GetText();
		void SetFont(FontInfo fontInfo);
		FontInfo GetFont();
		void SetFontSize(int size);
		int GetFontSize();
		void Show();
		void Hide();
		bool IsItShown();
		void SetTextColor(vec4u8 color);
		vec4u8 GetTextColor();
		void SetBorderColor(vec4u8 color);
		vec4u8 GetBorderColor();
		void SetDrawingBorder(bool draw = true);
		bool GetDrawingBorder();
		void TurnOnBack();
		void TurnOffBack();
		void SetWraped(bool isWraped);
		bool GetWraped();
		void SetBackgroundColor(vec4u8 color);
		vec4u8 GetBackgroundColor();
		void SetAlign(Alignment alignment);
		Alignment GetAlign();
		void SetVerticalIndent(int indent);
		void SetHorizontalIndent(int indent);
		int GetVerticalIndent();
		int GetHorizontalIndent();
		void SetBackImage(std::string key);
		void SetGeometry(vec4 geometry) override;
		vec4 GetGeometry();
		void Move(vec2 coords);

		void Draw() override;
	};
}