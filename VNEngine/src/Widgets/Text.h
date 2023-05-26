#pragma once

#include "Widgets/Widget.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <string>

namespace VNEngine {

	std::wstring cvt(const std::string& str);
	std::string cvtb(const std::wstring& str);

	struct textState {
		vec4 geometry;
		std::string text;
		std::string font, backimage;
		vec4u8 textcolor, backcolor,bordercolor;
		uint8_t align;
		bool shown, showborder, backisshown, wrapped;
		int hindent, vindent;
	};

	class Text : public Widget {

	protected:
		SDL_Texture* m_TextTexture;
		
		vec4 m_GeometryStart;
		vec4 m_TextNativeGeometry;
		vec4 m_TextDestination;
		int m_IndentVertical, m_IndentHorizontal;
		bool m_Wraped;

		vec4u8 m_TextColor;
		std::wstring m_Text;
		std::string m_FontKey;
		std::string m_BackImage;

		vec4u8 m_BorderColor;
		bool m_DrawBorder;

		void freeTexture();
		void windowResized();

	public:
		Text(vec4 geometry, std::wstring text = L"", vec4u8 textColor = {0,0,0,255},
			const std::string& fontKey = "");
		Text(const textState& ts);
		virtual ~Text();

		void SetText(std::wstring text);
		std::wstring GetText();
		void SetText(std::string text);
		void SetFont(std::string font);
		std::string GetFont();
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
		bool GetDrawingBack();
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
		std::string GetBackImage();
		void SetGeometry(vec4 geometry) override;
		vec4 GetGeometry();
		void Move(vec2 coords);

		void Draw() override;

		textState Dump();
	};
}