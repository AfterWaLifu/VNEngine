#pragma once

#include "Widgets/Text.h"

namespace VNEngine {

	class TextBox : public Text {

		static inline TextBox* s_ActiveTextBox;
		static inline std::string* s_InputString;

		std::string m_CurrentString;
		uint32_t m_MaxCharNumber;

		uint64_t m_LineTimer;
		bool m_LineEnabled;

	public:
		TextBox(vec4 geometry, std::wstring text = L"", uint32_t maxCharNumber = 32,
			vec4u8 textColor = { 0,0,0,255 }, FontInfo fontInfo = { "Roboto.ttf", 16 });
		~TextBox();

		void Check();
		void Draw();

	};
}