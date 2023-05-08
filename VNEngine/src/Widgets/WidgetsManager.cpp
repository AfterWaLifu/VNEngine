#include "WidgetsManager.h"
#include "vnepch.h"

#include "Core/Logger.h"

namespace VNEngine {

	WidgetsManager::WidgetsManager() : m_AllHiden(false)
	{
	}

	WidgetsManager::~WidgetsManager() {
	}

	WidgetsManager& WidgetsManager::Instance()
	{
		return s_pInstance ? *s_pInstance : *(s_pInstance = new WidgetsManager());
	}

	void WidgetsManager::AddWidget(const std::string& key, Text* text) {
		auto search = m_Widgets.m_AllTexts.find(key);
		if (!m_Widgets.m_AllTexts.empty() && search != m_Widgets.m_AllTexts.end()) {
			VN_LOGS_WARNING("Attemp to add Text widget with existing key");
			return;
		}
		m_Widgets.m_AllTexts[key] = std::shared_ptr<Text>(text);
	}

	void WidgetsManager::AddWidget(const std::string& key, Button* button) {
		auto search = m_Widgets.m_AllButtons.find(key);
		if (!m_Widgets.m_AllButtons.empty() && search != m_Widgets.m_AllButtons.end()) {
			VN_LOGS_WARNING("Attemp to add Button widget with existing key");
			return;
		}
		m_Widgets.m_AllButtons[key] = std::shared_ptr<Button>(button);
	}

	void WidgetsManager::AddWidget(const std::string& key, TextBox* textbox) {
		auto search = m_Widgets.m_AllTextBoxs.find(key);
		if (!m_Widgets.m_AllTextBoxs.empty() && search != m_Widgets.m_AllTextBoxs.end()) {
			VN_LOGS_WARNING("Attemp to add TextBox widget with existing key");
			return;
		}
		m_Widgets.m_AllTextBoxs[key] = std::shared_ptr<TextBox>(textbox);
	}

	void WidgetsManager::RemoveWidget(WidgetType type, const std::string& key) {
		switch (type) {
		case WIDGET_TEXT:
			m_Widgets.m_AllTexts.erase(key);
			break;
		case WIDGET_BUTTON:
			m_Widgets.m_AllButtons.erase(key);
			break;
		case WIDGET_TEXTBOX:
			m_Widgets.m_AllTextBoxs.erase(key);
			break;
		default:
			break;
		}
	}

	void WidgetsManager::WipeWidgets() {
		m_Widgets.m_AllTexts.clear();
		m_Widgets.m_AllButtons.clear();
		m_Widgets.m_AllTextBoxs.clear();
	}

	Text* WidgetsManager::GetText(const std::string& key) {
		auto search = m_Widgets.m_AllTexts.find(key);
		if (search == m_Widgets.m_AllTexts.end()) {
			VN_LOGS_WARNING("Attemp to find Text widget with non-existing key");
			return nullptr;
		}
		return m_Widgets.m_AllTexts[key].get();
	}

	Button* WidgetsManager::GetButton(const std::string& key) {
		auto search = m_Widgets.m_AllButtons.find(key);
		if (search == m_Widgets.m_AllButtons.end()) {
			VN_LOGS_WARNING("Attemp to find Button widget with non-existing key");
			return nullptr;
		}
		return m_Widgets.m_AllButtons[key].get();
	}

	TextBox* WidgetsManager::GetTextBox(const std::string& key) {
		auto search = m_Widgets.m_AllTextBoxs.find(key);
		if (search == m_Widgets.m_AllTextBoxs.end()) {
			VN_LOGS_WARNING("Attemp to find TextBox widget with non-existing key");
			return nullptr;
		}
		return m_Widgets.m_AllTextBoxs[key].get();
	}

	bool WidgetsManager::ExistsText(const std::string key) {
		if (m_Widgets.m_AllTexts.find(key) != m_Widgets.m_AllTexts.end()) return true;
		return false;
	}

	bool WidgetsManager::ExistsButton(const std::string key) {
		if (m_Widgets.m_AllButtons.find(key) != m_Widgets.m_AllButtons.end()) return true;
		return false;
	}

	bool WidgetsManager::ExistsTextBox(const std::string key) {
		if (m_Widgets.m_AllTextBoxs.find(key) != m_Widgets.m_AllTextBoxs.end()) return true;
		return false;
	}

	void WidgetsManager::Draw() {
		if (!m_Widgets.m_AllTexts.empty()) {
			for (auto t : m_Widgets.m_AllTexts) {
				t.second->Draw();
			}
		}
		if (!m_Widgets.m_AllButtons.empty()) {
			for (auto b : m_Widgets.m_AllButtons) {
				b.second->Draw();
			}
		}
		if (!m_Widgets.m_AllTextBoxs.empty()) {
			for (auto tb : m_Widgets.m_AllTextBoxs) {
				tb.second->Draw();
			}
		}
	}

	void WidgetsManager::Handle() {
		if (!m_Widgets.m_AllButtons.empty()) {
			for (auto b : m_Widgets.m_AllButtons) {
				b.second->Check();
			}
		}
		if (!m_Widgets.m_AllTextBoxs.empty()) {
			for (auto tb : m_Widgets.m_AllTextBoxs) {
				tb.second->Check();
			}
		}
	}

	void WidgetsManager::SetHiden(bool hide) {
		m_AllHiden = hide;
		if (m_AllHiden) {
			for (auto pair : m_Widgets.m_AllTexts) {
				pair.second->Hide();
			}
			for (auto pair : m_Widgets.m_AllButtons) {
				pair.second->Hide();
			}
			for (auto pair : m_Widgets.m_AllTextBoxs) {
				pair.second->Hide();
			}
		}
		else {
			for (auto pair : m_Widgets.m_AllTexts) {
				pair.second->Show();
			}
			for (auto pair : m_Widgets.m_AllButtons) {
				pair.second->Show();
			}
			for (auto pair : m_Widgets.m_AllTextBoxs) {
				pair.second->Show();
			}
		}
	}

	bool WidgetsManager::GetHiden() {
		return m_AllHiden;
	}

	void WidgetsManager::SaveScreen() {
		m_Screens.push_back(m_Widgets);
		WipeWidgets();
	}

	void WidgetsManager::PopScreen() {
		if (m_Screens.empty()) return;
		WipeWidgets();
		m_Widgets = m_Screens.back();
		m_Screens.pop_back();
	}

	void WidgetsManager::WipeScreens() {
		if (m_Screens.empty()) return;
		m_Screens.clear();
	}
}