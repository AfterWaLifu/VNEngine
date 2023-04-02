#pragma once

#include <SDL2/SDL.h>

#include <map>
#include <string>

#include "Core/Logger.h"

namespace VNEngine {

	namespace Keys {

		inline std::map<std::string, SDL_Scancode> KEYS = {
			{"a", SDL_SCANCODE_A},
			{"b", SDL_SCANCODE_B},
			{"c", SDL_SCANCODE_C},
			{"d", SDL_SCANCODE_D},
			{"e", SDL_SCANCODE_E},
			{"f", SDL_SCANCODE_F},
			{"g", SDL_SCANCODE_G},
			{"h", SDL_SCANCODE_H},
			{"i", SDL_SCANCODE_I},
			{"j", SDL_SCANCODE_J},
			{"k", SDL_SCANCODE_K},
			{"l", SDL_SCANCODE_L},
			{"m", SDL_SCANCODE_M},
			{"n", SDL_SCANCODE_N},
			{"o", SDL_SCANCODE_O},
			{"p", SDL_SCANCODE_P},
			{"q", SDL_SCANCODE_Q},
			{"r", SDL_SCANCODE_R},
			{"s", SDL_SCANCODE_S},
			{"t", SDL_SCANCODE_T},
			{"u", SDL_SCANCODE_U},
			{"v", SDL_SCANCODE_V},
			{"w", SDL_SCANCODE_W},
			{"x", SDL_SCANCODE_X},
			{"y", SDL_SCANCODE_Y},
			{"z", SDL_SCANCODE_Z},
			{"1", SDL_SCANCODE_1},
			{"2", SDL_SCANCODE_2},
			{"3", SDL_SCANCODE_3},
			{"4", SDL_SCANCODE_4},
			{"5", SDL_SCANCODE_5},
			{"6", SDL_SCANCODE_6},
			{"7", SDL_SCANCODE_7},
			{"8", SDL_SCANCODE_8},
			{"9", SDL_SCANCODE_9},
			{"0", SDL_SCANCODE_0},
			{"enter",		SDL_SCANCODE_RETURN},
			{"escape",		SDL_SCANCODE_ESCAPE},
			{"backspace",	SDL_SCANCODE_BACKSPACE},
			{"tab",			SDL_SCANCODE_TAB},
			{"space",		SDL_SCANCODE_SPACE},
			{"-",			SDL_SCANCODE_MINUS},
			{"=",			SDL_SCANCODE_EQUALS},
			{"(",			SDL_SCANCODE_LEFTBRACKET},
			{")",			SDL_SCANCODE_RIGHTBRACKET},
			{"\\",			SDL_SCANCODE_BACKSLASH},
			{";",			SDL_SCANCODE_SEMICOLON},
			{"'",			SDL_SCANCODE_APOSTROPHE},
			{"~",			SDL_SCANCODE_GRAVE},
			{",",			SDL_SCANCODE_COMMA},
			{">",			SDL_SCANCODE_PERIOD},
			{"/",			SDL_SCANCODE_SLASH},
			{"capslock",	SDL_SCANCODE_CAPSLOCK},
			{"f1",			SDL_SCANCODE_F1},
			{"f2",			SDL_SCANCODE_F2},
			{"f3",			SDL_SCANCODE_F3},
			{"f4",			SDL_SCANCODE_F4},
			{"f5",			SDL_SCANCODE_F5},
			{"f6",			SDL_SCANCODE_F6},
			{"f7",			SDL_SCANCODE_F7},
			{"f8",			SDL_SCANCODE_F8},
			{"f9",			SDL_SCANCODE_F9},
			{"f10",			SDL_SCANCODE_F10},
			{"f11",			SDL_SCANCODE_F11},
			{"f12",			SDL_SCANCODE_F12},
			{"printscreen",	SDL_SCANCODE_PRINTSCREEN},
			{"scrollock",	SDL_SCANCODE_SCROLLLOCK},
			{"pause",		SDL_SCANCODE_PAUSE},
			{"insert",		SDL_SCANCODE_INSERT},
			{"home",		SDL_SCANCODE_HOME},
			{"pageup",		SDL_SCANCODE_PAGEUP},
			{"delete",		SDL_SCANCODE_DELETE},
			{"end",			SDL_SCANCODE_END},
			{"pagedown",	SDL_SCANCODE_PAGEDOWN},
			{"right",		SDL_SCANCODE_RIGHT},
			{"left",		SDL_SCANCODE_LEFT},
			{"down",		SDL_SCANCODE_DOWN},
			{"up",			SDL_SCANCODE_UP},
			{"leftctrl",	SDL_SCANCODE_LCTRL},
			{"leftshift",	SDL_SCANCODE_LSHIFT},
			{"leftalt",		SDL_SCANCODE_LALT},
			{"leftwindows",	SDL_SCANCODE_LGUI},
			{"rightctrl",	SDL_SCANCODE_RCTRL},
			{"rightshift",	SDL_SCANCODE_RSHIFT},
			{"rightalt",	SDL_SCANCODE_RALT},
			{"rightwindows",SDL_SCANCODE_RGUI}
		};

		inline SDL_Scancode getScancodeFromKey(const char* key) {
			auto search = KEYS.find(key);
			if (search != KEYS.end()) return KEYS[key];
			else {
				VN_LOGS_WARNING("Can't find key '" << key << "'");
				return SDL_SCANCODE_UNKNOWN;
			}
		}
	}
}