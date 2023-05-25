#include "LuaReader.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "LuaReaders/LuaWidgetFunctions.h"
#include "LuaReaders/LuaSetsFunctions.h"
#include "LuaReaders/LuaFSMFunctions.h"

#include <LuaBridge/LuaBridge.h>

namespace VNEngine {
	
	LuaReader::LuaReader() {
		L = luaL_newstate();

		if (!L) {
			VN_LOGS_ERROR("Can't init Lua state, can't work >.<");
			return;
		}
		
		luaL_openlibs(L);
	}

	LuaReader::~LuaReader() {
		lua_close(L);
	}

	void LuaReader::RegisterInterfaceFunctions() {
		using namespace luabridge;

		getGlobalNamespace(L).
			beginNamespace("Widget").
			addFunction("Add",				LW::WidgetAdding).
			addFunction("Update",			LW::WidgetUpdate).
			addFunction("GetState",			LW::WidgetGetState).
			addFunction("Remove",			LW::WidgetRemove).
			addFunction("GetButtonClicked",	LW::WidgetButtonClicked).
			addFunction("GetButtonFocused",	LW::WidgetButtonFocused).
			addFunction("GetTextboxValue",	LW::WidgetTextboxValue).
			addFunction("SetDefChoiseButton",LW::SetDefaultChoiseButton).
			endNamespace();
	}

	void LuaReader::RegisterSettingsFunctons() {
		using namespace luabridge;

		getGlobalNamespace(L).
			beginNamespace("Game").
			addFunction("AddAudio", LS::AddAudio).
			addFunction("DeleteAudio", LS::DelAudio).
			addFunction("PlayMusic", LS::PlayMusic).
			addFunction("PauseMusic", LS::PauseMusic).
			addFunction("StopMysic", LS::StopMusic).
			addFunction("PlaySound", LS::PlaySound).
			addFunction("StopSound", LS::StopSound).
			addFunction("SetMusicVolume", LS::SetMusicVolume).
			addFunction("SetSoundVolume", LS::SetSoundVolume).
			addFunction("GetMusicVolume", LS::GetMusicVolume).
			addFunction("GetSoundVolume", LS::GetSoundVolume).
			addFunction("AudioMute", LS::Mute).
			addFunction("AudioUnmute", LS::Unmute).
			addFunction("AudioIsMuted", LS::IsMuted).
			addFunction("AddPicture", LS::AddPicture).
			addFunction("DelPicture", LS::DelPicture).
			addFunction("SetBackgroundColor", LS::SetBackgroundColor).
			addFunction("SetBackgroundPicture", LS::SetBackgroundPicture).
			addFunction("SetUsingBackgroundPic", LS::SetUsingBackgroundPic).
			addFunction("GetBackgroundColor", LS::GetBackgroundColor).
			addFunction("GetBackgroundPicture", LS::GetBackgroundPicture).
			addFunction("GetDrawingBackPicture", LS::GetDrawingBackPicture).
			addFunction("Draw", LS::Draw).
			addFunction("DrawByRowCollumn", LS::DrawRC).
			addFunction("StopDrawing", LS::StopDrawing).
			addFunction("StopDrawingById", LS::StopDrawingID).
			addFunction("WipeScreen", LS::WipeScreen).
			addFunction("SetWindowTitle", LS::SetWindowTitle).
			addFunction("SetWindowSize", LS::SetWindowSize).
			addFunction("GetWindowSize", LS::GetWindowSize).
			addFunction("Screenshot", LS::SaveScreenshot).
			addFunction("FontAdd", LS::FontAdd).
			addFunction("FontDel", LS::FontDel).
			addFunction("FontExist", LS::FontExist).
			addFunction("Exit", LS::ExitWithIH).
			endNamespace();
	}
	
	void LuaReader::RegisterFSMFunctuins() {
		using namespace luabridge;
		getGlobalNamespace(L).
			beginNamespace("Game").
			addFunction("Save", LSM::Save).
			addFunction("Load", LSM::Load).
			addFunction("ToMenu", LSM::ToMenu).
			addFunction("ToGame", LSM::GameStart).
			addFunction("IsThereReading", LSM::GetIfReadingExist).
			addFunction("WipeStates", LSM::WipeStates).
			endNamespace();
	}
}