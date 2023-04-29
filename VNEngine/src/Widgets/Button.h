#pragma once

#include "Widgets/Widget.h"
#include "Widgets/Text.h"
#include "Graphic/TextureManager.h"

#include <functional>

namespace VNEngine {

	class Button : public Text {

		std::function<void(void)> m_OnClick;
		bool m_Focused;

		vec4u8 m_DefaultBorder;
		vec4u8 m_FocusBorder;

	public:
		Button(vec4 geometry, std::function<void(void)> onClick = nullptr, std::wstring text = L"",
			vec4u8 BackgroundColor = {255,255,255,128}, vec4u8 textColor = {0,0,0,255},
			FontInfo fontInfo = { "Roboto.ttf", 16 });
		virtual ~Button();

		void Bind(std::function<void(void)> onClick);
		void Check();
		void Draw();
		bool Pressed();
		bool Focused();

		void SetBorderColor(vec4u8 color);
		void SetFocusBorderColor(vec4u8 color);
		vec4u8 GetFocusBorderColor();
	};
}


