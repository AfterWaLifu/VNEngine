#include "WidgetsManager.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	WidgetsManager::WidgetsManager()
	{
	}

	WidgetsManager::~WidgetsManager() {
	}

	void WidgetsManager::AddWidget(const std::string& key, Text* text) {
		auto search = m_AllTexts.find(key);
		if (!m_AllTexts.empty() && search == m_AllTexts.end()) {
			VN_LOGS_WARNING("Attemp to add Text widget with existing key");
			return;
		}
		m_AllTexts[key] = std::shared_ptr<Text>(text);
	}

	void WidgetsManager::AddWidget(const std::string& key, Button* button) {
		auto search = m_AllButtons.find(key);
		if (!m_AllButtons.empty() && search == m_AllButtons.end()) {
			VN_LOGS_WARNING("Attemp to add Button widget with existing key");
			return;
		}
		m_AllButtons[key] = std::shared_ptr<Button>(button);
	}

	void WidgetsManager::AddWidget(const std::string& key, TextBox* textbox) {
		auto search = m_AllTextBoxs.find(key);
		if (!m_AllTextBoxs.empty() && search == m_AllTextBoxs.end()) {
			VN_LOGS_WARNING("Attemp to add TextBox widget with existing key");
			return;
		}
		m_AllTextBoxs[key] = std::shared_ptr<TextBox>(textbox);
	}

	void WidgetsManager::RemoveWidget(WidgetType type, const std::string& key) {
		switch (type) {
		case WIDGET_TEXT:
			m_AllTexts.erase(key);
			break;
		case WIDGET_BUTTON:
			m_AllButtons.erase(key);
			break;
		case WIDGET_TEXTBOX:
			m_AllTextBoxs.erase(key);
			break;
		default:
			break;
		}
	}

	Text* WidgetsManager::GetText(const std::string& key) {
		auto search = m_AllTexts.find(key);
		if (search == m_AllTexts.end()) {
			VN_LOGS_WARNING("Attemp to find Text widget with non-existing key");
			return nullptr;
		}
		return m_AllTexts[key].get();
	}

	Button* WidgetsManager::GetButton(const std::string& key) {
		auto search = m_AllButtons.find(key);
		if (search == m_AllButtons.end()) {
			VN_LOGS_WARNING("Attemp to find Button widget with non-existing key");
			return nullptr;
		}
		return m_AllButtons[key].get();
	}

	TextBox* WidgetsManager::GetTextBox(const std::string& key) {
		auto search = m_AllTextBoxs.find(key);
		if (search == m_AllTextBoxs.end()) {
			VN_LOGS_WARNING("Attemp to find TextBox widget with non-existing key");
			return nullptr;
		}
		return m_AllTextBoxs[key].get();
	}

	void WidgetsManager::Draw() {
		if (!m_AllTexts.empty()) {
			for (auto t : m_AllTexts) {
				t.second->Draw();
			}
		}
		if (!m_AllButtons.empty()) {
			for (auto b : m_AllButtons) {
				b.second->Draw();
			}
		}
		if (!m_AllTextBoxs.empty()) {
			for (auto tb : m_AllTextBoxs) {
				tb.second->Draw();
			}
		}
	}

	void WidgetsManager::Handle() {
		if (!m_AllButtons.empty()) {
			for (auto b : m_AllButtons) {
				b.second->Check();
			}
		}
		if (!m_AllTextBoxs.empty()) {
			for (auto tb : m_AllTextBoxs) {
				tb.second->Check();
			}
		}
	}

}