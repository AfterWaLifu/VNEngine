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

		static inline WidgetsManager* s_pInstance;

		std::unordered_map<std::string, std::shared_ptr<Text>>		m_AllTexts;
		std::unordered_map<std::string, std::shared_ptr<Button>>	m_AllButtons;
		std::unordered_map<std::string, std::shared_ptr<TextBox>>	m_AllTextBoxs;

		WidgetsManager();
		~WidgetsManager();
	public:

		static WidgetsManager& Instance();

		enum WidgetType : uint8_t {
			WIDGET_TEXT = 0,
			WIDGET_BUTTON,
			WIDGET_TEXTBOX
		};

		void AddWidget(const std::string& key, Text* text);
		void AddWidget(const std::string& key, Button* button);
		void AddWidget(const std::string& key, TextBox* textbox);

		void RemoveWidget(WidgetType type, const std::string& key);

		Text* GetText(const std::string& key);
		Button* GetButton(const std::string& key);
		TextBox* GetTextBox(const std::string& key);

		void Draw();
		void Handle();
	};

#define WM_INSTANCE WidgetsManager::Instance()

}