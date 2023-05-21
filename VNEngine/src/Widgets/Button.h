#pragma once

#include "Widgets/Widget.h"
#include "Widgets/Text.h"
#include "Graphic/TextureManager.h"

#include <functional>

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace VNEngine {

	struct buttonState {
		textState ts;
		vec4u8 defaultborder, focusborder;
		luabridge::LuaRef* function;
	};

	class Button : public Text {

		std::function<void(void)> m_OnClick;
		luabridge::LuaRef* m_OnClickLua;
		bool m_Focused;

		vec4u8 m_DefaultBorder;
		vec4u8 m_FocusBorder;

	public:
		Button(vec4 geometry, std::function<void(void)> onClick = nullptr, std::wstring text = L"",
			vec4u8 BackgroundColor = {255,255,255,128}, vec4u8 textColor = {0,0,0,255},
			const std::string& fontKey = "");
		Button(const buttonState& bs);
		virtual ~Button();

		void Bind(std::function<void(void)> onClick);
		void Bind(luabridge::LuaRef onClick);
		void Check();
		void Draw();
		bool Pressed();
		bool Focused();

		void SetBorderColor(vec4u8 color);
		void SetFocusBorderColor(vec4u8 color);
		vec4u8 GetDefaultBorderColor();
		vec4u8 GetFocusBorderColor();

		buttonState Dump();
	};
}


