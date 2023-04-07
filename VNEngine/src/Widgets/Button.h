#pragma once

#include "Widgets/Widget.h"
#include "Widgets/Text.h"
#include "Graphic/TextureManager.h"

#include <functional>

namespace VNEngine {

	class Button : public Text {

		std::function<void(void)> m_OnClick;

	public:
		Button(vec4 geometry, std::function<void(void)> onClick = nullptr, std::wstring text = L"",
			vec4u8 BackgroundColor = {255,255,255,128}, vec4u8 textColor = {0,0,0,255},
			FontInfo fontInfo = { "Roboto.ttf", 16 });
		virtual ~Button();

		void Bind(std::function<void(void)> onClick);
		void Check(vec2 mousePos);
		void Draw();
	};
}


