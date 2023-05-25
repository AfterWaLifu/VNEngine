#include "LuaWidgetFunctions.h"
#include "vnepch.h"

#include "Widgets/WidgetsManager.h"
#include "Widgets/FontManager.h"
#include "Core/Logger.h"

#include "LuaReaders/LuaGameFunctions.h"

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
		bool tableIntoVec(vec4& vec, luabridge::LuaRef lr) {
			using namespace luabridge;
			if (lr.isTable() &&
				lr[1].isNumber() && lr[2].isNumber() &&
				lr[3].isNumber() && lr[4].isNumber()) {
				vec = {
					lr[1].cast<int>(), lr[2].cast<int>(),
					lr[3].cast<int>(), lr[4].cast<int>()
				};
				return true;
			}
			return false;
		}
		bool tableIntoVec(vec4u8& vec, luabridge::LuaRef lr) {
			if (lr.isTable() &&
				lr[1].isNumber() && lr[2].isNumber() &&
				lr[3].isNumber() && lr[4].isNumber()) {
				vec = {
					lr[1].cast<uint8_t>(), lr[2].cast<uint8_t>(),
					lr[3].cast<uint8_t>(), lr[4].cast<uint8_t>()
				};
				return true;
			}
			return false;
		}

		void WidgetAdding(luabridge::LuaRef t)
		{
			using namespace luabridge;

			if (t.isTable() && t["type"].isString()) {

				LuaRef geomTable = t["geometry"];
				vec4 geometry = {};
				if (!tableIntoVec(geometry, geomTable)) {
					geometry = { 0,0,100,100 };
				}

				std::string codename = "";
				if (t["name"].isString()) codename = t["name"].tostring();

				if (t["type"].tostring() == "text") {
					textState ts = {
						{ 0,0,100,100 }, "",FM_INSTANCE.GetDefaultFont(),
						"", {0,0,0,255}, { 0,0,0,0 },{ 0,0,0,0 },
						4|8, true, false,false,true,3,3
					};

					vec4u8 color = {};

					ts.geometry = geometry;
					if (t["text"].isString()) ts.text = t["text"].tostring();
					if (t["shown"].isBool() && !(t["shown"].cast<bool>())) ts.shown = false;
					if (t["font"].isString()) ts.font = t["font"].tostring();
					if (t["wrapped"].isBool()) ts.wrapped = t["wrapped"].cast<bool>();
					if (t["align"].isString()) {
						std::string align = t["align"].tostring();
						Alignment a;
						if (align[0] == 'L') a = ALIGN_LEFT;
						else if (align[0] == 'R') a = ALIGN_RIGHT;
						else a = ALIGN_HCENTER;
						if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
						else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
						else a = (Alignment)(a | ALIGN_VCENTER);
						ts.align = (uint8_t)a;
					}
					if (t["vindent"].isNumber()) ts.vindent = t["vindent"].cast<int>();
					if (t["hindent"].isNumber()) ts.hindent = t["hindent"].cast<int>();
					if (t["backPic"].isString()) ts.backimage = t["backPic"].tostring();
					if (t["textColor"].isTable()) {
						tableIntoVec(color, t["textColor"]);
						ts.textcolor = color;
					}
					if (t["backColor"].isTable()) {
						tableIntoVec(color, t["backColor"]);
						ts.backcolor = color;
					}
					if (t["borderColor"].isTable()) {
						tableIntoVec(color, t["borderColor"]);
						ts.bordercolor = color;
					}
					if (t["borderActive"].isBool()) ts.showborder = t["borderActive"].cast<bool>();

					WM_INSTANCE.AddWidget(codename, new Text(ts));
				}
				else if (t["type"].tostring() == "button") {
					buttonState bs = {
						{{ 0,0,100,100 }, "",FM_INSTANCE.GetDefaultFont(),
						"", {0,0,0,255}, {255,255,255,128},{ 215,215,215,255 },
						4 | 8, true, true,true,true,3,3 },
						{215,215,215,255},{175,175,175,255}, nullptr, nullptr
					};
					vec4u8 color = {};

					bs.ts.geometry = geometry;
					if (t["text"].isString()) bs.ts.text = t["text"].tostring();
					if (t["shown"].isBool() && !(t["shown"].cast<bool>())) bs.ts.shown = false;
					else bs.ts.shown = true;
					if (t["font"].isString()) bs.ts.font = t["font"].tostring();
					if (t["wrapped"].isBool()) bs.ts.wrapped = t["wrapped"].cast<bool>();
					if (t["align"].isString()) {
						std::string align = t["align"].tostring();
						Alignment a;
						if (align[0] == 'L') a = ALIGN_LEFT;
						else if (align[0] == 'R') a = ALIGN_RIGHT;
						else a = ALIGN_HCENTER;
						if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
						else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
						else a = (Alignment)(a | ALIGN_VCENTER);
						bs.ts.align = (uint8_t)a;
					}
					if (t["vindent"].isNumber()) bs.ts.vindent = t["vindent"].cast<int>();
					if (t["hindent"].isNumber()) bs.ts.hindent = t["hindent"].cast<int>();
					if (t["backPic"].isString()) bs.ts.backimage = t["backPic"].tostring();
					if (t["textColor"].isTable()) {
						tableIntoVec(color, t["textColor"]);
						bs.ts.textcolor = color;
					}
					if (t["backColor"].isTable()) {
						tableIntoVec(color, t["backColor"]);
						bs.ts.backcolor = color;
					}
					if (t["borderColor"].isTable()) {
						tableIntoVec(color, t["borderColor"]);
						bs.ts.bordercolor = color;
					}
					if (t["borderActive"].isBool()) bs.ts.showborder = t["borderActive"].cast<bool>();
					if (t["borderColorDefault"].isTable()) {
						tableIntoVec(color, t["borderColorDefault"]);
						bs.defaultborder = color;
					}
					if (t["borderColorFocus"].isTable()) {
						tableIntoVec(color, t["borderColorFocus"]);
						bs.focusborder = color;
					}
					if (t["func"].isFunction()) bs.functionLua = new LuaRef(t["func"]);
					
					WM_INSTANCE.AddWidget(codename, new Button(bs));
				}
				else if (t["type"].tostring() == "textbox") {
					textboxState tbs = {
						{{ 0,0,100,100 }, "",FM_INSTANCE.GetDefaultFont(),
						"", {0,0,0,255}, {255,255,255,128},{ 215,215,215,255 },
						4 | 8, true, true,true,true,3,3 },
						"",32
					};
					vec4u8 color = {};

					tbs.ts.geometry = geometry;
					if (t["text"].isString()) tbs.ts.text = t["text"].tostring();
					if (t["shown"].isBool() && !(t["shown"].cast<bool>())) tbs.ts.shown = false;
					else tbs.ts.shown = true;
					if (t["font"].isString()) tbs.ts.font = t["font"].tostring();
					if (t["wrapped"].isBool()) tbs.ts.wrapped = t["wrapped"].cast<bool>();
					if (t["align"].isString()) {
						std::string align = t["align"].tostring();
						Alignment a;
						if (align[0] == 'L') a = ALIGN_LEFT;
						else if (align[0] == 'R') a = ALIGN_RIGHT;
						else a = ALIGN_HCENTER;
						if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
						else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
						else a = (Alignment)(a | ALIGN_VCENTER);
						tbs.ts.align = (uint8_t)a;
					}
					if (t["vindent"].isNumber()) tbs.ts.vindent = t["vindent"].cast<int>();
					if (t["hindent"].isNumber()) tbs.ts.hindent = t["hindent"].cast<int>();
					if (t["backPic"].isString()) tbs.ts.backimage = t["backPic"].tostring();
					if (t["textColor"].isTable()) {
						tableIntoVec(color, t["textColor"]);
						tbs.ts.textcolor = color;
					}
					if (t["backColor"].isTable()) {
						tableIntoVec(color, t["backColor"]);
						tbs.ts.backcolor = color;
					}
					if (t["borderColor"].isTable()) {
						tableIntoVec(color, t["borderColor"]);
						tbs.ts.bordercolor = color;
					}
					if (t["borderActive"].isBool()) tbs.ts.showborder = t["borderActive"].cast<bool>();
					if (t["currentString"].isString()) tbs.currentString = t["currentString"].tostring();
					if (t["maxChar"].isNumber()) tbs.maxchar = t["maxChar"].cast<uint32_t>();

					WM_INSTANCE.AddWidget(codename, new TextBox(tbs));
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

			if (text) {
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
					else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
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
			else if (button) {
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
					else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
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
			else if (textbox) {
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
					else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
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
			WidgetsManager::WidgetType t = (WidgetsManager::WidgetType) 255;
			if (type == std::string("text")) t = WidgetsManager::WIDGET_TEXT;
			else if (type == std::string("button")) t = WidgetsManager::WIDGET_BUTTON;
			else if (type == std::string("textbox")) t = WidgetsManager::WIDGET_TEXTBOX;
			if ((uint8_t)t == 255) return;
			WM_INSTANCE.RemoveWidget(t, name);
		}

		luabridge::LuaRef WidgetGetState(std::string type, std::string name, lua_State* L) {
			using namespace luabridge;
			LuaRef table(L, newTable(L));

			if (type != "text" && type != "button" && type != "textbox") return table;

			Text* t = nullptr;
			Button* b = nullptr;
			TextBox* tb = nullptr;

			if (type == "text") t = WM_INSTANCE.GetText(name);
			if (type == "button") b = WM_INSTANCE.GetButton(name);
			if (type == "textbox") tb = WM_INSTANCE.GetTextBox(name);

			if (t) {
				textState ts = {};
				ts = t->Dump();
				table["shown"] = ts.shown;
				table["geometry"] = vecIntoTable(ts.geometry, L);
				table["textColor"] = vecIntoTable(ts.textcolor, L);
				table["text"] = ts.text;
				table["wrapped"] = ts.wrapped;
				table["align"] = ts.align;
				table["vindent"] = ts.vindent;
				table["hindent"] = ts.hindent;
				table["font"] = ts.font;
				table["backPic"] = ts.backimage;
				table["backColor"] = vecIntoTable(ts.backcolor, L);
				table["borderColor"] = vecIntoTable(ts.bordercolor, L);
				table["borderActive"] = ts.showborder;
			}
			else if (b) {
				buttonState bs = b->Dump();
				table["shown"] = bs.ts.shown;
				table["geometry"] = vecIntoTable(bs.ts.geometry, L);
				table["textColor"] = vecIntoTable(bs.ts.textcolor, L);
				table["text"] = bs.ts.text;
				table["wrapped"] = bs.ts.wrapped;
				table["align"] = bs.ts.align;
				table["vindent"] = bs.ts.vindent;
				table["hindent"] = bs.ts.hindent;
				table["font"] = bs.ts.font;
				table["backPic"] = bs.ts.backimage;
				table["backColor"] = vecIntoTable(bs.ts.backcolor, L);
				table["borderColor"] = vecIntoTable(bs.ts.bordercolor, L);
				table["borderActive"] = bs.ts.showborder;
				table["borderColorDefault"] = vecIntoTable(bs.defaultborder, L);
				table["borderColorFocus"] = vecIntoTable(bs.focusborder, L);
				table["func"] = bs.function;
			}
			else if (tb) {
				textboxState tbs = {};
				tbs = tb->Dump();
				table["shown"] = tbs.ts.shown;
				table["geometry"] = vecIntoTable(tbs.ts.geometry, L);
				table["textColor"] = vecIntoTable(tbs.ts.textcolor, L);
				table["text"] = tbs.ts.text;
				table["wrapped"] = tbs.ts.wrapped;
				table["align"] = tbs.ts.align;
				table["vindent"] = tbs.ts.vindent;
				table["hindent"] = tbs.ts.hindent;
				table["font"] = tbs.ts.font;
				table["backPic"] = tbs.ts.backimage;
				table["backColor"] = vecIntoTable(tbs.ts.backcolor, L);
				table["borderColor"] = vecIntoTable(tbs.ts.bordercolor, L);
				table["borderActive"] = tbs.ts.showborder;
				table["currentString"] = tbs.currentString;
				table["maxChar"] = tbs.maxchar;
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

		void SetDefaultChoiseButton(luabridge::LuaRef t) {
			buttonState bs = {
				{{ 0,0,0,0 }, "",FM_INSTANCE.GetDefaultFont(),
				"", {0,0,0,255}, {255,255,255,128},{ 215,215,215,255 },
				4 | 8, true, true,true,true,3,3 },
				{215,215,215,255},{175,175,175,255}, nullptr, nullptr
			};
			bool withGeom = false;
			if (t.isTable()) {
				vec4u8 color = {};
				if (t["font"].isString()) bs.ts.font = t["font"].tostring();
				if (t["wrapped"].isBool()) bs.ts.wrapped = t["wrapped"].cast<bool>();
				if (t["align"].isString()) {
					std::string align = t["align"].tostring();
					Alignment a;
					if (align[0] == 'L') a = ALIGN_LEFT;
					else if (align[0] == 'R') a = ALIGN_RIGHT;
					else a = ALIGN_HCENTER;
					if (align[1] == 'T') a = (Alignment)(a | ALIGN_UP);
					else if (align[1] == 'B') a = (Alignment)(a | ALIGN_DOWN);
					else a = (Alignment)(a | ALIGN_VCENTER);
					bs.ts.align = (uint8_t)a;
				}
				if (t["vindent"].isNumber()) bs.ts.vindent = t["vindent"].cast<int>();
				if (t["hindent"].isNumber()) bs.ts.hindent = t["hindent"].cast<int>();
				if (t["backPic"].isString()) bs.ts.backimage = t["backPic"].tostring();
				if (t["textColor"].isTable()) {
					tableIntoVec(color, t["textColor"]);
					bs.ts.textcolor = color;
				}
				if (t["backColor"].isTable()) {
					tableIntoVec(color, t["backColor"]);
					bs.ts.backcolor = color;
				}
				if (t["borderColor"].isTable()) {
					tableIntoVec(color, t["borderColor"]);
					bs.ts.bordercolor = color;
				}
				if (t["borderActive"].isBool()) bs.ts.showborder = t["borderActive"].cast<bool>();
				if (t["borderColorDefault"].isTable()) {
					tableIntoVec(color, t["borderColorDefault"]);
					bs.defaultborder = color;
				}
				if (t["borderColorFocus"].isTable()) {
					tableIntoVec(color, t["borderColorFocus"]);
					bs.focusborder = color;
				}
			}
			LG::setDefaultChooseButton(&bs,withGeom);
		}
	}
}

