#include "LuaWidgetFunctions.h"
#include "vnepch.h"

#include "Widgets/WidgetsManager.h"
#include "Core/Logger.h"

namespace VNEngine {
	
	namespace LW {
		luabridge::LuaRef vecIntoTable(vec4 vec, lua_State* L) {
			using namespace luabridge;
			LuaRef table(L, newTable(L));

			table[1] = vec.x;
			table[2] = vec.y;
			table[3] = vec.w;
			table[4] = vec.h;
			return table;
		}
		luabridge::LuaRef vecIntoTable(vec4u8 vec, lua_State* L) {
			using namespace luabridge;
			LuaRef table(L, newTable(L));

			table[1] = vec.r;
			table[2] = vec.g;
			table[3] = vec.b;
			table[4] = vec.a;
			return table;
		}

		void WidgetAdding(luabridge::LuaRef t)
		{
			using namespace luabridge;

			if (t.isTable() && t["type"].isString()) {

				auto geomTable = t["geometry"];
				vec4 geometry = {};
				if (geomTable.isTable() &&
					geomTable[1].isNumber() && geomTable[2].isNumber() &&
					geomTable[3].isNumber() && geomTable[4].isNumber()) {
					geometry = {
						geomTable[1].cast<int>(), geomTable[2].cast<int>(),
						geomTable[3].cast<int>(), geomTable[4].cast<int>()
					};
				}
				else {
					geometry = { 0,0,100,100 };
				}

				std::string codename = "";
				std::wstring text = L"";
				if (t["name"].isString()) codename = t["name"].tostring();
				text = cvt(t["text"].tostring());

				if (t["type"].tostring() == "text") {
					Text* textWidget = new Text(geometry, text);
					if (t["shown"].isBool() && !(t["shown"].cast<bool>())) textWidget->Hide();
					if (t["font"].isString()) textWidget->SetFont(t["font"].tostring());
					if (t["wrapped"].isBool() && !(t["wrapped"].cast<bool>())) textWidget->SetWraped(t["wrapped"].cast<bool>());
					if (t["align"].isString()) {
						std::string align = t["align"].tostring();
						Alignment a;
						if (align[0] == 'L') a = ALIGN_LEFT;
						else if (align[0] == 'R') a = ALIGN_RIGHT;
						else a = ALIGN_HCENTER;
						if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
						else if (align[1] == 'D') a = (Alignment)(a | ALIGN_DOWN);
						else a = (Alignment)(a | ALIGN_VCENTER);
						textWidget->SetAlign(a);
					}
					if (t["vindent"].isNumber()) textWidget->SetVerticalIndent(t["vindent"].cast<int>());
					if (t["hindent"].isNumber()) textWidget->SetHorizontalIndent(t["hindent"].cast<int>());
					if (t["backPic"].isString()) textWidget->SetBackImage(t["backPic"].tostring());
					if (t["textColor"].isTable() &&
						t["textColor"][1].isNumber() && t["textColor"][2].isNumber() &&
						t["textColor"][3].isNumber() && t["textColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["textColor"][1].cast<uint8_t>() , t["textColor"][2].cast<uint8_t>(),
							t["textColor"][3].cast<uint8_t>() , t["textColor"][4].cast<uint8_t>()
						};
						textWidget->SetTextColor(backcolor);
					}
					if (t["backColor"].isTable() &&
						t["backColor"][1].isNumber() && t["backColor"][2].isNumber() &&
						t["backColor"][3].isNumber() && t["backColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["backColor"][1].cast<uint8_t>() , t["backColor"][2].cast<uint8_t>(),
							t["backColor"][3].cast<uint8_t>() , t["backColor"][4].cast<uint8_t>()
						};
						textWidget->SetBackgroundColor(backcolor);
					}
					if (t["borderColor"].isTable() &&
						t["borderColor"][1].isNumber() && t["borderColor"][2].isNumber() &&
						t["borderColor"][3].isNumber() && t["borderColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["borderColor"][1].cast<uint8_t>() , t["borderColor"][2].cast<uint8_t>(),
							t["borderColor"][3].cast<uint8_t>() , t["borderColor"][4].cast<uint8_t>()
						};
						textWidget->SetBorderColor(backcolor);
					}
					if (t["borderActive"].isBool() && t["borderActive"].cast<bool>()) textWidget->SetDrawingBorder();

					WM_INSTANCE.AddWidget(codename, textWidget);
				}
				else if (t["type"].tostring() == "button") {
					Button* buttonWidget = new Button(geometry, nullptr, text);
					if (t["shown"].isBool() && !(t["shown"].cast<bool>())) buttonWidget->Hide();
					if (t["font"].isString()) buttonWidget->SetFont(t["font"].tostring());
					if (t["wrapped"].isBool() && !(t["wrapped"].cast<bool>())) buttonWidget->SetWraped(t["wrapped"].cast<bool>());
					if (t["align"].isString()) {
						std::string align = t["align"].tostring();
						Alignment a;
						if (align[0] == 'L') a = ALIGN_LEFT;
						else if (align[0] == 'R') a = ALIGN_RIGHT;
						else a = ALIGN_HCENTER;
						if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
						else if (align[1] == 'D') a = (Alignment)(a | ALIGN_DOWN);
						else a = (Alignment)(a | ALIGN_VCENTER);
						buttonWidget->SetAlign(a);
					}
					if (t["vindent"].isNumber()) buttonWidget->SetVerticalIndent(t["vindent"].cast<int>());
					if (t["hindent"].isNumber()) buttonWidget->SetHorizontalIndent(t["hindent"].cast<int>());
					if (t["backPic"].isString()) buttonWidget->SetBackImage(t["backPic"].tostring());
					if (t["textColor"].isTable() &&
						t["textColor"][1].isNumber() && t["textColor"][2].isNumber() &&
						t["textColor"][3].isNumber() && t["textColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["textColor"][1].cast<uint8_t>() , t["textColor"][2].cast<uint8_t>(),
							t["textColor"][3].cast<uint8_t>() , t["textColor"][4].cast<uint8_t>()
						};
						buttonWidget->SetTextColor(backcolor);
					}
					if (t["backColor"].isTable() &&
						t["backColor"][1].isNumber() && t["backColor"][2].isNumber() &&
						t["backColor"][3].isNumber() && t["backColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["backColor"][1].cast<uint8_t>() , t["backColor"][2].cast<uint8_t>(),
							t["backColor"][3].cast<uint8_t>() , t["backColor"][4].cast<uint8_t>()
						};
						buttonWidget->SetBackgroundColor(backcolor);
					}
					if (t["borderColor"].isTable() &&
						t["borderColor"][1].isNumber() && t["borderColor"][2].isNumber() &&
						t["borderColor"][3].isNumber() && t["borderColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["borderColor"][1].cast<uint8_t>() , t["borderColor"][2].cast<uint8_t>(),
							t["borderColor"][3].cast<uint8_t>() , t["borderColor"][4].cast<uint8_t>()
						};
						buttonWidget->SetBorderColor(backcolor);
					}
					if (t["borderFocusColor"].isTable() &&
						t["borderFocusColor"][1].isNumber() && t["borderFocusColor"][2].isNumber() &&
						t["borderFocusColor"][3].isNumber() && t["borderFocusColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["borderFocusColor"][1].cast<uint8_t>() , t["borderFocusColor"][2].cast<uint8_t>(),
							t["borderFocusColor"][3].cast<uint8_t>() , t["borderFocusColor"][4].cast<uint8_t>()
						};
						buttonWidget->SetFocusBorderColor(backcolor);
					}
					if (t["borderActive"].isBool() && !(t["borderActive"].cast<bool>())) buttonWidget->SetDrawingBorder(false);

					WM_INSTANCE.AddWidget(codename, buttonWidget);
				}
				else if (t["type"].tostring() == "textbox") {
					TextBox* textBoxWidget = new TextBox(geometry, text);
					if (t["shown"].isBool() && !(t["shown"].cast<bool>())) textBoxWidget->Hide();
					if (t["font"].isString()) textBoxWidget->SetFont(t["font"].tostring());
					if (t["wrapped"].isBool() && !(t["wrapped"].cast<bool>())) textBoxWidget->SetWraped(t["wrapped"].cast<bool>());
					if (t["align"].isString()) {
						std::string align = t["align"].tostring();
						Alignment a;
						if (align[0] == 'L') a = ALIGN_LEFT;
						else if (align[0] == 'R') a = ALIGN_RIGHT;
						else a = ALIGN_HCENTER;
						if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
						else if (align[1] == 'D') a = (Alignment)(a | ALIGN_DOWN);
						else a = (Alignment)(a | ALIGN_VCENTER);
						textBoxWidget->SetAlign(a);
					}
					if (t["vindent"].isNumber()) textBoxWidget->SetVerticalIndent(t["vindent"].cast<int>());
					if (t["hindent"].isNumber()) textBoxWidget->SetHorizontalIndent(t["hindent"].cast<int>());
					if (t["backPic"].isString()) textBoxWidget->SetBackImage(t["backPic"].tostring());
					if (t["textColor"].isTable() &&
						t["textColor"][1].isNumber() && t["textColor"][2].isNumber() &&
						t["textColor"][3].isNumber() && t["textColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["textColor"][1].cast<uint8_t>() , t["textColor"][2].cast<uint8_t>(),
							t["textColor"][3].cast<uint8_t>() , t["textColor"][4].cast<uint8_t>()
						};
						textBoxWidget->SetTextColor(backcolor);
					}
					if (t["backColor"].isTable() &&
						t["backColor"][1].isNumber() && t["backColor"][2].isNumber() &&
						t["backColor"][3].isNumber() && t["backColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["backColor"][1].cast<uint8_t>() , t["backColor"][2].cast<uint8_t>(),
							t["backColor"][3].cast<uint8_t>() , t["backColor"][4].cast<uint8_t>()
						};
						textBoxWidget->SetBackgroundColor(backcolor);
					}
					if (t["borderColor"].isTable() &&
						t["borderColor"][1].isNumber() && t["borderColor"][2].isNumber() &&
						t["borderColor"][3].isNumber() && t["borderColor"][4].isNumber()) {
						vec4u8 backcolor = {
							t["borderColor"][1].cast<uint8_t>() , t["borderColor"][2].cast<uint8_t>(),
							t["borderColor"][3].cast<uint8_t>() , t["borderColor"][4].cast<uint8_t>()
						};
						textBoxWidget->SetBorderColor(backcolor);
					}
					if (t["borderActive"].isBool() && !(t["borderActive"].cast<bool>())) textBoxWidget->SetDrawingBorder(false);
					if (t["maxCharQuantity"].isNumber()) textBoxWidget->SetMaxCharNumber(t["maxCharQuantity"].cast<uint32_t>());

					WM_INSTANCE.AddWidget(codename, textBoxWidget);
				}
			}
			else {
				VN_LOGS_WARNING("Couldn't create widget" << t.tostring());
			}
		}

		void WidgetUpdate(std::string type, std::string name, luabridge::LuaRef t)
		{
			Text* text = nullptr;
			Button* button = nullptr;
			TextBox* textbox = nullptr;
			if (type == std::string("text")) {
				text = WM_INSTANCE.GetText(name);
			}
			else if (type == std::string("button")) {
				button = WM_INSTANCE.GetButton(name);
			}
			else if (type == std::string("textbox")) {
				textbox = WM_INSTANCE.GetTextBox(name);
			}

			if (text != nullptr) {
				if (t["shown"].isBool() && !(t["shown"].cast<bool>())) text->Hide();
				if (t["font"].isString()) text->SetFont(t["font"].tostring());
				if (t["wrapped"].isBool() && !(t["wrapped"].cast<bool>())) text->SetWraped(t["wrapped"].cast<bool>());
				if (t["align"].isString()) {
					std::string align = t["align"].tostring();
					Alignment a;
					if (align[0] == 'L') a = ALIGN_LEFT;
					else if (align[0] == 'R') a = ALIGN_RIGHT;
					else a = ALIGN_HCENTER;
					if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
					else if (align[1] == 'D') a = (Alignment)(a | ALIGN_DOWN);
					else a = (Alignment)(a | ALIGN_VCENTER);
					text->SetAlign(a);
				}
				if (t["vindent"].isNumber()) text->SetVerticalIndent(t["vindent"].cast<int>());
				if (t["hindent"].isNumber()) text->SetHorizontalIndent(t["hindent"].cast<int>());
				if (t["backPic"].isString()) text->SetBackImage(t["backPic"].tostring());
				if (t["textColor"].isTable() &&
					t["textColor"][1].isNumber() && t["textColor"][2].isNumber() &&
					t["textColor"][3].isNumber() && t["textColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["textColor"][1].cast<uint8_t>() , t["textColor"][2].cast<uint8_t>(),
						t["textColor"][3].cast<uint8_t>() , t["textColor"][4].cast<uint8_t>()
					};
					text->SetTextColor(backcolor);
				}
				if (t["backColor"].isTable() &&
					t["backColor"][1].isNumber() && t["backColor"][2].isNumber() &&
					t["backColor"][3].isNumber() && t["backColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["backColor"][1].cast<uint8_t>() , t["backColor"][2].cast<uint8_t>(),
						t["backColor"][3].cast<uint8_t>() , t["backColor"][4].cast<uint8_t>()
					};
					text->SetBackgroundColor(backcolor);
				}
				if (t["borderColor"].isTable() &&
					t["borderColor"][1].isNumber() && t["borderColor"][2].isNumber() &&
					t["borderColor"][3].isNumber() && t["borderColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["borderColor"][1].cast<uint8_t>() , t["borderColor"][2].cast<uint8_t>(),
						t["borderColor"][3].cast<uint8_t>() , t["borderColor"][4].cast<uint8_t>()
					};
					text->SetBorderColor(backcolor);
				}
				if (t["borderActive"].isBool() && t["borderActive"].cast<bool>()) text->SetDrawingBorder();
			}
			else if (button != nullptr) {
				if (t["shown"].isBool() && !(t["shown"].cast<bool>())) button->Hide();
				if (t["font"].isString()) button->SetFont(t["font"].tostring());
				if (t["wrapped"].isBool() && !(t["wrapped"].cast<bool>())) button->SetWraped(t["wrapped"].cast<bool>());
				if (t["align"].isString()) {
					std::string align = t["align"].tostring();
					Alignment a;
					if (align[0] == 'L') a = ALIGN_LEFT;
					else if (align[0] == 'R') a = ALIGN_RIGHT;
					else a = ALIGN_HCENTER;
					if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
					else if (align[1] == 'D') a = (Alignment)(a | ALIGN_DOWN);
					else a = (Alignment)(a | ALIGN_VCENTER);
					button->SetAlign(a);
				}
				if (t["vindent"].isNumber()) button->SetVerticalIndent(t["vindent"].cast<int>());
				if (t["hindent"].isNumber()) button->SetHorizontalIndent(t["hindent"].cast<int>());
				if (t["backPic"].isString()) button->SetBackImage(t["backPic"].tostring());
				if (t["textColor"].isTable() &&
					t["textColor"][1].isNumber() && t["textColor"][2].isNumber() &&
					t["textColor"][3].isNumber() && t["textColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["textColor"][1].cast<uint8_t>() , t["textColor"][2].cast<uint8_t>(),
						t["textColor"][3].cast<uint8_t>() , t["textColor"][4].cast<uint8_t>()
					};
					button->SetTextColor(backcolor);
				}
				if (t["backColor"].isTable() &&
					t["backColor"][1].isNumber() && t["backColor"][2].isNumber() &&
					t["backColor"][3].isNumber() && t["backColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["backColor"][1].cast<uint8_t>() , t["backColor"][2].cast<uint8_t>(),
						t["backColor"][3].cast<uint8_t>() , t["backColor"][4].cast<uint8_t>()
					};
					button->SetBackgroundColor(backcolor);
				}
				if (t["borderColor"].isTable() &&
					t["borderColor"][1].isNumber() && t["borderColor"][2].isNumber() &&
					t["borderColor"][3].isNumber() && t["borderColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["borderColor"][1].cast<uint8_t>() , t["borderColor"][2].cast<uint8_t>(),
						t["borderColor"][3].cast<uint8_t>() , t["borderColor"][4].cast<uint8_t>()
					};
					button->SetBorderColor(backcolor);
				}
				if (t["borderFocusColor"].isTable() &&
					t["borderFocusColor"][1].isNumber() && t["borderFocusColor"][2].isNumber() &&
					t["borderFocusColor"][3].isNumber() && t["borderFocusColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["borderFocusColor"][1].cast<uint8_t>() , t["borderFocusColor"][2].cast<uint8_t>(),
						t["borderFocusColor"][3].cast<uint8_t>() , t["borderFocusColor"][4].cast<uint8_t>()
					};
					button->SetFocusBorderColor(backcolor);
				}
				if (t["borderActive"].isBool() && !(t["borderActive"].cast<bool>())) button->SetDrawingBorder(false);
			}
			else if (textbox != nullptr) {
				if (t["shown"].isBool() && !(t["shown"].cast<bool>())) textbox->Hide();
				if (t["font"].isString()) textbox->SetFont(t["font"].tostring());
				if (t["wrapped"].isBool() && !(t["wrapped"].cast<bool>())) textbox->SetWraped(t["wrapped"].cast<bool>());
				if (t["align"].isString()) {
					std::string align = t["align"].tostring();
					Alignment a;
					if (align[0] == 'L') a = ALIGN_LEFT;
					else if (align[0] == 'R') a = ALIGN_RIGHT;
					else a = ALIGN_HCENTER;
					if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
					else if (align[1] == 'D') a = (Alignment)(a | ALIGN_DOWN);
					else a = (Alignment)(a | ALIGN_VCENTER);
					textbox->SetAlign(a);
				}
				if (t["vindent"].isNumber()) textbox->SetVerticalIndent(t["vindent"].cast<int>());
				if (t["hindent"].isNumber()) textbox->SetHorizontalIndent(t["hindent"].cast<int>());
				if (t["backPic"].isString()) textbox->SetBackImage(t["backPic"].tostring());
				if (t["textColor"].isTable() &&
					t["textColor"][1].isNumber() && t["textColor"][2].isNumber() &&
					t["textColor"][3].isNumber() && t["textColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["textColor"][1].cast<uint8_t>() , t["textColor"][2].cast<uint8_t>(),
						t["textColor"][3].cast<uint8_t>() , t["textColor"][4].cast<uint8_t>()
					};
					textbox->SetTextColor(backcolor);
				}
				if (t["backColor"].isTable() &&
					t["backColor"][1].isNumber() && t["backColor"][2].isNumber() &&
					t["backColor"][3].isNumber() && t["backColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["backColor"][1].cast<uint8_t>() , t["backColor"][2].cast<uint8_t>(),
						t["backColor"][3].cast<uint8_t>() , t["backColor"][4].cast<uint8_t>()
					};
					textbox->SetBackgroundColor(backcolor);
				}
				if (t["borderColor"].isTable() &&
					t["borderColor"][1].isNumber() && t["borderColor"][2].isNumber() &&
					t["borderColor"][3].isNumber() && t["borderColor"][4].isNumber()) {
					vec4u8 backcolor = {
						t["borderColor"][1].cast<uint8_t>() , t["borderColor"][2].cast<uint8_t>(),
						t["borderColor"][3].cast<uint8_t>() , t["borderColor"][4].cast<uint8_t>()
					};
					textbox->SetBorderColor(backcolor);
				}
				if (t["borderActive"].isBool() && !(t["borderActive"].cast<bool>())) textbox->SetDrawingBorder(false);
				if (t["maxCharQuantity"].isNumber()) textbox->SetMaxCharNumber(t["maxCharQuantity"].cast<uint32_t>());
			}
		}

		void WidgetRemove(std::string type, std::string name) {
			WidgetsManager::WidgetType t = WidgetsManager::WIDGET_TEXT;
			if (type == std::string("button")) t = WidgetsManager::WIDGET_BUTTON;
			else if (type == std::string("textbox")) t = WidgetsManager::WIDGET_TEXTBOX;
			WM_INSTANCE.RemoveWidget(t, name);
		}

		luabridge::LuaRef WidgetGetState(std::string type, std::string name, lua_State* L) {
			using namespace luabridge;
			LuaRef table(L, newTable(L));

			Text* text = nullptr;
			Button* button = nullptr;
			TextBox* textbox = nullptr;
			if (type == std::string("text")) {
				text = WM_INSTANCE.GetText(name);
			}
			else if (type == std::string("button")) {
				button = WM_INSTANCE.GetButton(name);
			}
			else if (type == std::string("textbox")) {
				textbox = WM_INSTANCE.GetTextBox(name);
			}

			if (text != nullptr) {
				table["shown"] = text->IsItShown();
				table["geometry"] = vecIntoTable(text->GetGeometry(), L);
				table["textColor"] = vecIntoTable(text->GetTextColor(), L);
				table["text"] = text->GetText();
				table["wrapped"] = text->GetWraped();
				table["align"] = text->GetAlign();
				table["vindent"] = text->GetVerticalIndent();
				table["hindent"] = text->GetHorizontalIndent();
				table["font"] = text->GetFont();
				table["backPic"] = text->GetBackImage();
				table["backColor"] = vecIntoTable(text->GetBackgroundColor(), L);
				table["borderColor"] = vecIntoTable(text->GetBorderColor(), L);
				table["borderActive"] = text->GetDrawingBorder();
			}
			else if (button != nullptr) {
				table["shown"] = button->IsItShown();
				table["geometry"] = vecIntoTable(button->GetGeometry(), L);
				table["textColor"] = vecIntoTable(button->GetTextColor(), L);
				table["text"] = button->GetText();
				table["wrapped"] = text->GetWraped();
				table["align"] = button->GetAlign();
				table["vindent"] = button->GetVerticalIndent();
				table["hindent"] = button->GetHorizontalIndent();
				table["font"] = button->GetFont();
				table["backPic"] = button->GetBackImage();
				table["backColor"] = vecIntoTable(button->GetBackgroundColor(), L);
				table["borderColor"] = vecIntoTable(button->GetBorderColor(), L);
				table["borderActive"] = button->GetDrawingBorder();
				table["borderFocusColor"] = vecIntoTable(button->GetFocusBorderColor(), L);
			}
			else if (textbox != nullptr) {
				table["shown"] = textbox->IsItShown();
				table["geometry"] = vecIntoTable(textbox->GetGeometry(), L);
				table["textColor"] = vecIntoTable(textbox->GetTextColor(), L);
				table["text"] = textbox->GetText();
				table["wrapped"] = text->GetWraped();
				table["align"] = textbox->GetAlign();
				table["vindent"] = textbox->GetVerticalIndent();
				table["hindent"] = textbox->GetHorizontalIndent();
				table["font"] = textbox->GetFont();
				table["backPic"] = textbox->GetBackImage();
				table["backColor"] = vecIntoTable(textbox->GetBackgroundColor(), L);
				table["borderColor"] = vecIntoTable(textbox->GetBorderColor(), L);
				table["borderActive"] = textbox->GetDrawingBorder();
				table["maxCharQuantity"] = textbox->GetMaxCharNumber();
			}

			return table;
		}

		bool WidgetButtonClicked(std::string name) {
			return WM_INSTANCE.GetButton(name)->Pressed();
		}

		bool WidgetButtonFocused(std::string name) {
			return WM_INSTANCE.GetButton(name)->Focused();
		}

		std::string WidgetTextboxValue(std::string name) {
			return WM_INSTANCE.GetTextBox(name)->Value();
		}
	}
}

