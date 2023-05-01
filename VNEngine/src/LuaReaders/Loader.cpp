#include "Loader.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "Core/App.h"
#include "Audio/AudioPlayer.h"
#include "Controls/InputHandler.h"
#include "Graphic/TextureManager.h"
#include "Widgets/FontManager.h"
#include "LuaReaders/StoryTeller.h"

#include <LuaBridge/LuaBridge.h>

#define lb	luabridge

namespace VNEngine {
	
	void Loader::readPresets() {
		if (luaL_dofile(L, "game/presets.lua")) {
			VN_LOGS_WARNING("Cant load Lua presets");
			return;
		}
		lb::LuaRef t = lb::getGlobal(L, "scripts");
		if (t.isString()) {
			StoryTeller::SetScriptPath(t.tostring());
		}
		t = lb::getGlobal(L, "images");
		if (t.isString()) {
			TextureManager::SetImagesPath(t.tostring());
		}
		t = lb::getGlobal(L, "fonts");
		if (t.isString()) {
			FM_INSTANCE.SetFontPath(t.tostring());
		}
		t = lb::getGlobal(L, "fontDefault");
		if (t.isString()) {
			FM_INSTANCE.SetDefaultFont(t.tostring());
		}
		t = lb::getGlobal(L, "audio");
		if (t.isString()) {
			AP_INSTANCE.SetAudiofilePath(t.tostring());
		}
	}

	void Loader::readSettings() {
		std::string title = "Title";
		int width = 1280, height = 720;
		bool foolscreen = false;
		
		if (luaL_dofile(L, ((StoryTeller::GetScriptsPath() + "settings.lua").c_str()))) {
			VN_LOGS_ERROR("Cant load settings");
		}
		lb::LuaRef t = lb::getGlobal(L, "Window");
		if (t.isTable()) {
			if (t["Title"].isString())	title = t["Title"].cast<std::string>();
			if (t["Width"].isNumber())	width = t["Width"].cast<int>();
			if (t["Height"].isNumber())	height= t["Height"].cast<int>();
			if (t["Fullscreen"].isBool()) foolscreen = t["Fullscreen"].cast<bool>();
		}
		m_pApp->m_Drawer = std::make_unique<Artist>(
			title, width, height, foolscreen);
		if (!(t.isNil()) && t["Resizable"].isBool()) 
			m_pApp->m_Drawer->SetWindowResizable(t["Resizable"].cast<bool>());
		
		vec4 vecColor = { 0,0,0,255 };
		uint8_t stretching = 2;
		t = lb::getGlobal(L, "Background");
		if (!(t.isNil())) {
			if (!(t["Color"].isNil()) && t["Color"].isTable()) {
				lb::LuaRef color = t["Color"];
				if (color[1].isNumber()) vecColor.r = color[1].cast<int>();
				if (color[2].isNumber()) vecColor.g = color[2].cast<int>();
				if (color[3].isNumber()) vecColor.b = color[3].cast<int>();
				if (color[4].isNumber()) vecColor.a = color[4].cast<int>();
			}
			if (t["Stretching"].isString()) {
				std::string stretch = t["Stretching"].cast<std::string>();
				if (stretch == "centered") stretching = 0;
				else if (stretch == "fullscreen") stretching = 1;
				else stretching = 2;
			}
		}
		if (vecColor.r != 0 || vecColor.g != 0 || vecColor.b != 0 || vecColor.a != 255) {
			vec4u8 shortenedColor = {
				(uint8_t)(vecColor.r % 256), (uint8_t)(vecColor.g % 256),
				(uint8_t)(vecColor.b % 256), (uint8_t)(vecColor.a % 256)
			};
			m_pApp->m_Drawer->SetBackground(shortenedColor);
		}
		if (stretching != 2) m_pApp->m_Drawer->SetStretchingState((Stretching)stretching);
	}

	Loader::Loader(App* app) : LuaReader()
	{
		m_pApp = app;
		readPresets();
		readSettings();
		InputHandler::InputHandlerInit(&(m_pApp->m_IsRunning));
		State::InitDrawingForStates(m_pApp->m_Drawer.get());
	}

	Loader::~Loader() {
		
	}
}
