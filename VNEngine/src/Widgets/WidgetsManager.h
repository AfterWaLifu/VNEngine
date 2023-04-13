#pragma once

#include "Widgets/Widget.h"
#include "Widgets/Text.h"
#include "Widgets/Button.h"
#include "Widgets/TextBox.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace VNEngine {

	class WidgetsManager {

		std::unordered_map<std::string, std::shared_ptr<Text>>		m_AllTexts;
		std::unordered_map<std::string, std::shared_ptr<Button>>	m_AllButtons;
		std::unordered_map<std::string, std::shared_ptr<TextBox>>	m_AllTextBoxs;

	public:
		WidgetsManager();
		~WidgetsManager();

		void AddWidget(std::string key, Text* text);
		void AddWidget(std::string key, Button* button);
		void AddWidget(std::string key, TextBox* textbox);

		Text* GetText(std::string key);
		Button* GetButton(std::string key);
		TextBox* GetTextBox(std::string key);

		void Draw();
		void Handle();
	};

}