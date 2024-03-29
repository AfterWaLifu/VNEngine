#pragma once

#include "Widgets/Text.h"

namespace VNEngine {

	struct textboxState {
		textState ts;
		std::string currentString;
		uint32_t maxchar;
	};

	class TextBox : public Text {

		static inline TextBox* s_ActiveTextBox;
		static inline std::string* s_InputString;

		std::string m_CurrentString;
		uint32_t m_MaxCharNumber;

		uint64_t m_LineTimer;
		bool m_LineEnabled;

	public:
		TextBox(vec4 geometry, std::wstring text = L"", uint32_t maxCharNumber = 32,
			vec4u8 textColor = { 0,0,0,255 }, const std::string& fontKey = "");
		TextBox(const textboxState& tbs);
		~TextBox();

		void Check();
		void Draw();

		std::string Value();
		void SetValue(const std::string& str);
		void SetMaxCharNumber(uint32_t quantity);
		uint32_t GetMaxCharNumber();

		textboxState Dump();
	};
}