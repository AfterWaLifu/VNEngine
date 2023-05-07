#include "LuaSetsFunctions.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "Audio/AudioPlayer.h"
#include "Graphic/Artist.h"
#include "Controls/InputHandler.h"
#include "Widgets/FontManager.h"
#include "StateMachine/StateMachine.h"

namespace VNEngine {

	namespace LS {
		static inline Artist* s_ArtistToDraw;

		luabridge::LuaRef vecIntoTable(vec4u8 vec, lua_State* L) {
			using namespace luabridge;
			LuaRef table(L, newTable(L));

			table[1] = vec.r;
			table[2] = vec.g;
			table[3] = vec.b;
			table[4] = vec.a;
			return table;
		}

		void InitDrawingForLua(Artist* artist) {
			s_ArtistToDraw = artist;
		}

		void AddAudio(std::string filename, std::string key) {
			AP_INSTANCE.AddAudio(filename, key);
		}

		void DelAudio(std::string key) {
			AP_INSTANCE.RemoveAudio(key);
		}

		void PlayMusic(std::string key) {
			AP_INSTANCE.PlayMusic(key);
		}

		void PauseMusic() {
			AP_INSTANCE.PauseMusic();
		}

		void StopMusic() {
			AP_INSTANCE.StopMusic();
		}

		void PlaySound(std::string key) {
			AP_INSTANCE.PlaySound(key);
		}

		void StopSound() {
			AP_INSTANCE.StopSound();
		}

		void SetMusicVolume(float v) {
			AP_INSTANCE.SetMusicVolume(v);
		}

		void SetSoundVolume(float v) {
			AP_INSTANCE.SetSoundVolume(v);
		}

		float GetMusicVolume() {
			return AP_INSTANCE.GetMusicVolume();
		}

		float GetSoundVolume() {
			return AP_INSTANCE.GetSoundVolume();
		}

		void Mute() {
			AP_INSTANCE.Mute();
		}

		void Unmute() {
			AP_INSTANCE.Unmute();
		}

		void AddPicture(std::string filename, std::string key, int rows, int collumns) {
			s_ArtistToDraw->AddTexture(key, filename);
		}

		void DelPicture(std::string key) {
			s_ArtistToDraw->DeleteTexture(key);
		}
		
		void SetBackgroundColor(luabridge::LuaRef color) {
			using namespace luabridge;
			if (color.isTable() && 
				color[1].isNumber() && color[2].isNumber() &&
				color[3].isNumber() && color[4].isNumber()) {
				vec4u8 vecColor = {
					color[1].cast<uint8_t>(), color[2].cast<uint8_t>(),
					color[3].cast<uint8_t>(), color[4].cast<uint8_t>()
				};
				s_ArtistToDraw->SetBackground(vecColor);
			}
			else {
				VN_LOGS_WARNING("Attemp to set a color with a thing below" << color.tostring());
			}
		}
		
		void SetBackgroundPicture(std::string key) {
			s_ArtistToDraw->SetBackground(key);
		}
		
		void SetUsingBackgroundPic(bool trueIfPicture) {
			s_ArtistToDraw->SetUsingBackgroundPic(trueIfPicture);
		}
		
		luabridge::LuaRef GetBackgroundColor(lua_State* L) {
			return vecIntoTable(s_ArtistToDraw->GetBackgroundColor(), L);
		}
		
		std::string GetBackgroundPicture() {
			return s_ArtistToDraw->GetBackgroundPic();
		}
		
		bool GetDrawingBackPicture() {
			return s_ArtistToDraw->GetDrawingPictureOrColor();
		}
		
		uint32_t Draw(std::string key, luabridge::LuaRef destination, int tileNum) {
			vec4 dest = {};
			if (destination.isTable() && 
				destination[1].isNumber() && destination[2].isNumber() &&
				destination[3].isNumber() && destination[4].isNumber()) {
				dest = {
					destination[1].cast<int>(), destination[2].cast<int>(),
					destination[3].cast<int>(), destination[4].cast<int>()
				};
			}
			else {
				VN_LOGS_WARNING("Attemp to draw w/out destination");
			}

			return s_ArtistToDraw->Draw(key, tileNum, dest);
		}
		
		uint32_t DrawRC(std::string key, luabridge::LuaRef destination, int row, int collumn) {
			vec4 dest = {};
			if (destination.isTable() &&
				destination[1].isNumber() && destination[2].isNumber() &&
				destination[3].isNumber() && destination[4].isNumber()) {
				dest = {
					destination[1].cast<int>(), destination[2].cast<int>(),
					destination[3].cast<int>(), destination[4].cast<int>()
				};
			}
			else {
				VN_LOGS_WARNING("Attemp to draw w/out destination");
			}

			return s_ArtistToDraw->Draw(key, row, collumn, dest);
		}
		
		void StopDrawing(std::string key) {
			s_ArtistToDraw->StopDrawing(key);
		}
		
		void StopDrawingID(uint32_t id) {
			s_ArtistToDraw->StopDrawing(id);
		}
		
		void WipeScreen() {
			s_ArtistToDraw->WipeDrawing();
		}
		
		void SetWindowTitle(std::string title) {
			s_ArtistToDraw->SetWindowTitle(title);
		}

		void SetWindowSize(luabridge::LuaRef size) {
			vec2 vecSize = {};
			if (size.isTable() &&
				size[1].isNumber() && size[2].isNumber()){
				vecSize = { size[1].cast<int>(), size[2].cast<int>() };
			}
			else {
				VN_LOGS_WARNING("Attemp to resize window w/out destination");
			}

			s_ArtistToDraw->SetWindowSize(vecSize);
		}

		luabridge::LuaRef GetWindowSize(lua_State* L) {
			using namespace luabridge;
			LuaRef table(L, newTable(L));
			vec2 size = s_ArtistToDraw->GetWindowSize();
			table[1] = size.x;
			table[2] = size.y;
			return table;
		}
		
		void SaveScreenshot() {
			s_ArtistToDraw->SaveScreenshot();
		}

		void FontAdd(std::string key, std::string filename, int size) {
			FM_INSTANCE.addFont(key, filename, size);
		}

		void FontDel(std::string key) {
			FM_INSTANCE.delFont(key);
		}

		bool FontExist(std::string key) {
			return FM_INSTANCE.Exist(key);
		}

		bool GetIfReadingExist() {
			return SM_INSTANCE.isThereAReading();
		}

		void ExitWithIH() {
			IH_INSTANCE.exit();
		}
	}
}