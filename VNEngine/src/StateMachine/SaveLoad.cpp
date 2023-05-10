#include "SaveLoad.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "Audio/AudioPlayer.h"
#include "Graphic/Artist.h"
#include "Widgets/WidgetsManager.h"
#include "StateMachine/StateMachine.h"

#include <json/json.h>

#include <filesystem>

namespace VNEngine {

	void writeInVec(Json::Value* list, const vec4* geom) {
		Json::Value temp = geom->x;
		list->append(temp);
		temp = geom->y;
		list->append(temp);
		temp = geom->w;
		list->append(temp);
		temp = geom->h;
		list->append(temp);
	}
	void writeInVec(Json::Value* list, vec4u8* color) {
		Json::Value temp = color->r;
		list->append(temp);
		temp = color->g;
		list->append(temp);
		temp = color->b;
		list->append(temp);
		temp = color->a;
		list->append(temp);
	}

	void SaveLoad::Save(int number, Artist* partist) {
		std::ofstream save((sSaveDir + std::to_string(number) + ".vns"),
			std::fstream::out | std::fstream::trunc);
		if (!save.is_open()) {
			VN_LOGS_WARNING("Error on opening saving file for save, check presests.lua save path");
			char buff[256];
			strerror_s(buff, errno);
			VN_LOGS_WARNING(buff);
			return;
		}

		AudioPlayer::dump ad = AP_INSTANCE.Dump();
		Artist::dump dd = partist->Dumb();
		WidgetsManager::dump wd = WM_INSTANCE.Dump();
		size_t sp = (size_t)SM_INSTANCE.GetTopReaderPos();

		Json::Value Root;
		Json::Value Audio;
		Json::Value Graphic;
		Json::Value Widgets;
		Json::Value List(Json::arrayValue);
		Json::Value tempList(Json::arrayValue);
		Json::Value q(Json::arrayValue);
		Json::Value v;
		Json::Value z;
		
		//	AUDIO
		Audio["mkey"] = ad.s.mkey;
		Audio["skey"] = ad.s.skey;
		Audio["plays"] = ad.s.plays;
		for (auto pair : ad.list) {
			v = pair.first;
			tempList.append(v);
			v = pair.second;
			tempList.append(v);
			List.append(tempList);
			tempList.clear();
		}
		Audio["list"] = List;
		List.clear();

		//	GRAPHICS
		for (auto t : dd.list) {
			v = t.filename;
			tempList.append(v);
			v = t.key;
			tempList.append(v);
			v = t.r;
			tempList.append(v);
			v = t.c;
			tempList.append(v);
			List.append(tempList);
			tempList.clear();
		}
		Graphic["list"] = List;
		List.clear();
		z["textureKey"] = dd.s.textureKey;
		z["colorChanged"] = dd.s.colorChanged;
		z["drawBackPic"] = dd.s.drawBackPic;
		z["id"] = dd.s.id;
		z["stretchState"] = (uint8_t)dd.s.stretchState;
		writeInVec(&List, &dd.s.dest);
		z["dest"] = List;
		List.clear();
		writeInVec(&List, &dd.s.backgroundColor);
		z["backgroundColor"] = List;
		List.clear();
		for (const auto& pair : dd.s.q) {
			v = pair.first;
			List.append(v);
			v = pair.second.texture->key;
			List.append(v);
			writeInVec(&tempList, &(pair.second.source));
			List.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &(pair.second.destination));
			List.append(tempList);
			tempList.clear();
			q.append(List);
		}
		z["q"] = q;
		Graphic["Screen"] = z;

		List.clear();
		tempList.clear();
		z.clear();
		q.clear();

		//	WIDGETS
		for (auto w : wd.ts) {
			v = w.text.c_str();
			q.append(v);
			writeInVec(&tempList,&w.geometry);
			q.append(tempList);
			tempList.clear();
			v = w.font;
			q.append(v);
			v = w.backimage;
			q.append(v);
			writeInVec(&tempList, &w.textcolor);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.backcolor);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.bordercolor);
			q.append(tempList);
			tempList.clear();
			v = w.align;
			q.append(v);
			v = w.shown;
			q.append(v);
			v = w.showborder;
			q.append(v);
			v = w.backisshown;
			q.append(v);
			v = w.wrapped;
			q.append(v);
			v = w.hindent;
			q.append(v);
			v = w.vindent;
			q.append(v);
			List.append(q);
			q.clear();
		}
		Widgets["Texts"] = List;
		List.clear();
		q.clear();
		for (auto w : wd.bs) {
			v = w.ts.text.c_str();
			q.append(v);
			writeInVec(&tempList, &w.ts.geometry);
			q.append(tempList);
			tempList.clear();
			v = w.ts.font;
			q.append(v);
			v = w.ts.backimage;
			q.append(v);
			writeInVec(&tempList, &w.ts.textcolor);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.ts.backcolor);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.ts.bordercolor);
			q.append(tempList);
			tempList.clear();
			v = w.ts.align;
			q.append(v);
			v = w.ts.shown;
			q.append(v);
			v = w.ts.showborder;
			q.append(v);
			v = w.ts.backisshown;
			q.append(v);
			v = w.ts.wrapped;
			q.append(v);
			v = w.ts.hindent;
			q.append(v);
			v = w.ts.vindent;
			q.append(v);
			writeInVec(&tempList, &w.defaultborder);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.focusborder);
			q.append(tempList);
			tempList.clear();
			List.append(q);
			q.clear();
		}
		Widgets["Buttons"] = List;
		List.clear();
		q.clear();
		for (auto w : wd.tbs) {
			v = w.ts.text.c_str();
			q.append(v);
			writeInVec(&tempList, &w.ts.geometry);
			q.append(tempList);
			tempList.clear();
			v = w.ts.font;
			q.append(v);
			v = w.ts.backimage;
			q.append(v);
			writeInVec(&tempList, &w.ts.textcolor);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.ts.backcolor);
			q.append(tempList);
			tempList.clear();
			writeInVec(&tempList, &w.ts.bordercolor);
			q.append(tempList);
			tempList.clear();
			v = w.ts.align;
			q.append(v);
			v = w.ts.shown;
			q.append(v);
			v = w.ts.showborder;
			q.append(v);
			v = w.ts.backisshown;
			q.append(v);
			v = w.ts.wrapped;
			q.append(v);
			v = w.ts.hindent;
			q.append(v);
			v = w.ts.vindent;
			q.append(v);
			v = w.currentString;
			q.append(v);
			v = w.maxchar;
			q.append(v);
			List.append(q);
			q.clear();
		}
		Widgets["TextBoxes"] = List;
		List.clear();
		q.clear();

		//	SAVING
		Root["Audio"] = Audio;
		Root["Graphic"] = Graphic;
		Root["Widgets"] = Widgets;
		Root["ReaderPosition"] = sp;

		Json::StreamWriterBuilder builder;
		const std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		writer->write(Root, &save);
		save.close();
	}
	
	void SaveLoad::Load(int number) {

	}
	
	void SaveLoad::SetSaveDir(const std::string& path) {
		sSaveDir = path;
	}
	
	std::string SaveLoad::GetSaveDir() {
		return sSaveDir;
	}
}