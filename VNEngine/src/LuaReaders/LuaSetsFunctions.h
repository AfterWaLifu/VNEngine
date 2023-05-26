#pragma once

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

namespace VNEngine {
	class Artist;

	namespace LS {
		void InitDrawingForLua(Artist* artist);

		//	audioplayer control functions
		void AddAudio(std::string filename, std::string key);
		void DelAudio(std::string key);
		void PlayMusic(std::string key);
		void PauseMusic();
		void StopMusic();
		void PlaySound(std::string key);
		void StopSound();
		void SetMusicVolume(float v);
		void SetSoundVolume(float v);
		float GetMusicVolume();
		float GetSoundVolume();
		void Mute();
		void Unmute();
		bool IsMuted();

		//	drawer control functions
		void AddPicture(std::string filename, std::string key, int rows = 1, int collumns = 1);
		void DelPicture(std::string key);
		void SetBackgroundColor(luabridge::LuaRef color);
		void SetBackgroundPicture(std::string key);
		void SetUsingBackgroundPic(bool trueIfPicture);
		luabridge::LuaRef GetBackgroundColor(lua_State* L);
		std::string GetBackgroundPicture();
		bool GetDrawingBackPicture();
		uint32_t Draw(luabridge::LuaRef key, luabridge::LuaRef destination,
			luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil);
		uint32_t DrawLeft(luabridge::LuaRef key, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil);
		uint32_t DrawCenter(luabridge::LuaRef key, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil);
		uint32_t DrawRight(luabridge::LuaRef key, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil);
		void StopDrawing(std::string key);
		void StopDrawingID(uint32_t id);
		void WipeScreen();
		void SetWindowTitle(std::string title);
		void SetWindowSize(luabridge::LuaRef size);
		luabridge::LuaRef GetWindowSize(lua_State* L);
		void SaveScreenshot();

		//	font functions
		void FontAdd(std::string key, std::string filename, int size);
		void FontDel(std::string key);
		bool FontExist(std::string key);

		void ExitWithIH();
	}
}