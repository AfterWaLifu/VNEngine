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
		bool tableIntoVec(vec2& vec, luabridge::LuaRef lr) {
			using namespace luabridge;
			if (lr.isTable() && lr[1].isNumber() && lr[2].isNumber()) {
				vec = { lr[1].cast<int>(), lr[1].cast<int>() };
				return true;
			}
			else return false;
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
			if (AP_INSTANCE.GetIfMusicPlays() && AP_INSTANCE.GetCurrentMusic() == key) {
				return;
			}
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

		bool IsMuted() {
			return AP_INSTANCE.GetIfMute();
		}

		void AddPicture(std::string filename, std::string key, int rows, int collumns) {
			s_ArtistToDraw->AddTexture(key, filename, rows, collumns);
		}

		void DelPicture(std::string key) {
			s_ArtistToDraw->DeleteTexture(key);
		}
		
		void SetBackgroundColor(luabridge::LuaRef color) {
			using namespace luabridge;
			vec4u8 colorvec{};
			if (tableIntoVec(colorvec, color)) {
				s_ArtistToDraw->SetBackground(colorvec);
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
		
		uint32_t Draw(luabridge::LuaRef key, luabridge::LuaRef destination, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil) {
			if (key.isString() && destination.isTable()) {
				vec4 dest4 = {};
				vec4 dest2 = {};
				if (destination[1].isNumber() && destination[2].isNumber() &&
					destination[1].isNumber() && destination[4].isNumber()) {
					tableIntoVec(dest4, destination);
					if (tileOrRow.isNil() && collumnOrNil.isNil()) {
						return s_ArtistToDraw->DrawR(key.tostring(), 0, dest4);
					}
					if (tileOrRow.isNumber() && collumnOrNil.isNumber()) {
						return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(),
							collumnOrNil.cast<int>(), dest4);
					}
					else if (tileOrRow.isNumber()) {
						return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(), dest4);
					}
				}
				else if (destination[1].isNumber() && destination[2].isNumber()) {
					dest2 = { destination[1].cast<int>(),destination[2].cast<int>() };
					auto t = TM_INSTANCE.getTexture(key.tostring());
					if (!t) return UINT32_MAX;
					dest2.w = t->w, dest2.h = t->h;
					if (tileOrRow.isNil() && collumnOrNil.isNil()) {
						return s_ArtistToDraw->DrawR(key.tostring(), 0, dest2);
					}
					if (tileOrRow.isNumber() && collumnOrNil.isNumber()) {
						return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(),
							collumnOrNil.cast<int>(), dest2);
					}
					else if (tileOrRow.isNumber()) {
						return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(), dest2);
					}
				}
				{
					VN_LOGS_WARNING("To draw destination table of 2 (xy) or 4(xywh) is needed");
					return UINT32_MAX;
				}
			}
			else {
				VN_LOGS_WARNING("To draw key and destination are needed");
				return UINT32_MAX;
			}
		}

		uint32_t DrawLeft(luabridge::LuaRef key, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil) {
			if (key.isString()) {
				auto winsize = s_ArtistToDraw->GetWindowSize();
				auto t = TM_INSTANCE.getTexture(key.tostring());
				vec4 dest = {
					(winsize.x / 2 - (t->w/t->c)) / 2,
					(t->h < winsize.y / 4) ? winsize.y * 3 - (t->h / t->r) / 4 : winsize.y - (t->h / t->r),
					t->w / t->c,
					t->h / t->r
				};
				if (tileOrRow.isNil() && collumnOrNil.isNil()) {
					return s_ArtistToDraw->DrawR(key.tostring(), 0, dest);
				}
				if (tileOrRow.isNumber() && collumnOrNil.isNumber()) {
					return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(),
						collumnOrNil.cast<int>(), dest);
				}
				else if (tileOrRow.isNumber()) {
					return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(), dest);
				}
				else {
					VN_LOGS_WARNING("To draw destination table of 2 (xy) or 4(xywh) is needed");
					return UINT32_MAX;
				}
			}
			else {
				VN_LOGS_WARNING("To draw key and destination are needed");
				return UINT32_MAX;
			}
		}

		uint32_t DrawCenter(luabridge::LuaRef key, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil) {
			if (key.isString()) {
				auto winsize = s_ArtistToDraw->GetWindowSize();
				auto t = TM_INSTANCE.getTexture(key.tostring());
				vec4 dest = {
					(winsize.x - (t->w / t->c)) / 2,
					(t->h < winsize.y / 4) ? winsize.y * 3 - (t->h/t->r) / 4 : winsize.y - (t->h/t->r),
					t->w / t->c,
					t->h / t->r
				};
				if (tileOrRow.isNil() && collumnOrNil.isNil()) {
					return s_ArtistToDraw->DrawR(key.tostring(), 0, dest);
				}
				if (tileOrRow.isNumber() && collumnOrNil.isNumber()) {
					return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(),
						collumnOrNil.cast<int>(), dest);
				}
				else if (tileOrRow.isNumber()) {
					return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(), dest);
				}
				else {
					VN_LOGS_WARNING("To draw destination table of 2 (xy) or 4(xywh) is needed");
					return UINT32_MAX;
				}
			}
			else {
				VN_LOGS_WARNING("To draw key and destination are needed");
				return UINT32_MAX;
			}
		}

		uint32_t DrawRight(luabridge::LuaRef key, luabridge::LuaRef tileOrRow, luabridge::LuaRef collumnOrNil) {
			if (key.isString()) {
				auto winsize = s_ArtistToDraw->GetWindowSize();
				auto t = TM_INSTANCE.getTexture(key.tostring());
				vec4 dest = {
					(winsize.x * 3 / 2 - (t->w/(t->c))) / 2,
					(t->h < winsize.y / 4) ? winsize.y * 3 - (t->h / t->r) / 4 : winsize.y - (t->h / t->r),
					t->w / t->c,
					t->h / t->r
				};
				if (tileOrRow.isNil() && collumnOrNil.isNil()) {
					return s_ArtistToDraw->DrawR(key.tostring(), 0, dest);
				}
				if (tileOrRow.isNumber() && collumnOrNil.isNumber()) {
					return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(),
						collumnOrNil.cast<int>(), dest);
				}
				else if (tileOrRow.isNumber()) {
					return s_ArtistToDraw->DrawR(key.tostring(), tileOrRow.cast<int>(), dest);
				}
				else {
					VN_LOGS_WARNING("To draw destination table of 2 (xy) or 4(xywh) is needed");
					return UINT32_MAX;
				}
			}
			else {
				VN_LOGS_WARNING("To draw key and destination are needed");
				return UINT32_MAX;
			}
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

		void ExitWithIH() {
			IH_INSTANCE.exit();
		}
	}
}