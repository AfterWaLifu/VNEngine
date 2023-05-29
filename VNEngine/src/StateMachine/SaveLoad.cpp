#include "SaveLoad.h"
#include "vnepch.h"

#include "Core/Logger.h"
#include "Audio/AudioPlayer.h"
#include "Graphic/Artist.h"
#include "Widgets/WidgetsManager.h"
#include "StateMachine/StateMachine.h"

#include <tinyxml2.h>
#include <SDL2/SDL_image.h>

#include <filesystem>

namespace VNEngine {

	void vectoxml(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* elem, const vec4& vec) {
		using namespace tinyxml2;
		XMLElement* x = doc.NewElement("x");
		XMLElement* y = doc.NewElement("y");
		XMLElement* w = doc.NewElement("w");
		XMLElement* h = doc.NewElement("h");
		x->SetText(vec.x);
		y->SetText(vec.y);
		w->SetText(vec.w);
		h->SetText(vec.h);
		elem->InsertEndChild(x);
		elem->InsertEndChild(y);
		elem->InsertEndChild(w);
		elem->InsertEndChild(h);
	}
	void vectoxml(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* elem, vec4u8 vec) {
		using namespace tinyxml2;
		XMLElement* r = doc.NewElement("r");
		XMLElement* g = doc.NewElement("g");
		XMLElement* b = doc.NewElement("b");
		XMLElement* a = doc.NewElement("a");
		r->SetText(vec.r);
		g->SetText(vec.g);
		b->SetText(vec.b);
		a->SetText(vec.a);
		elem->InsertEndChild(r);
		elem->InsertEndChild(g);
		elem->InsertEndChild(b);
		elem->InsertEndChild(a);
	}
	vec4 xmltovec(tinyxml2::XMLElement* elem) {
		vec4 geom = {
			atoi(elem->FirstChildElement("x")->GetText()),
			atoi(elem->FirstChildElement("y")->GetText()),
			atoi(elem->FirstChildElement("w")->GetText()),
			atoi(elem->FirstChildElement("h")->GetText())
		};
		return geom;
	}
	vec4u8 xmltouvec(tinyxml2::XMLElement* elem) {
		vec4u8 color = {
			(uint8_t)atoi(elem->FirstChildElement("r")->GetText()),
			(uint8_t)atoi(elem->FirstChildElement("g")->GetText()),
			(uint8_t)atoi(elem->FirstChildElement("b")->GetText()),
			(uint8_t)atoi(elem->FirstChildElement("a")->GetText())
		};
		return color;
	}

	void dealWithAudio(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) {
		using namespace tinyxml2;

		AudioPlayer::dump ad = AP_INSTANCE.Dump();
		XMLElement* audio = doc.NewElement("Audio");

		XMLElement* audioList = doc.NewElement("List");
		for (int i = 0; i < ad.list.size(); ++i) {
			XMLElement* listElem = doc.NewElement("a");
			XMLElement* filename = doc.NewElement("filename");
			std::string f = ad.list[i].first;
			size_t offset = f.rfind("/");
			f = f.substr(offset+1, f.size()-offset-1);
			filename->SetText(f.c_str());
			XMLElement* key = doc.NewElement("key");
			key->SetText(ad.list[i].second.c_str());
			listElem->InsertFirstChild(filename);
			listElem->InsertEndChild(key);
			audioList->InsertEndChild(listElem);
		}
		XMLElement* audioState = doc.NewElement("State");
		XMLElement* mkey = doc.NewElement("mkey");
		XMLElement* skey = doc.NewElement("skey");
		XMLElement* mplays = doc.NewElement("mplays");
		XMLElement* splays = doc.NewElement("splays");
		mkey->SetText(ad.s.mkey.c_str());
		skey->SetText(ad.s.skey.c_str());
		mplays->SetText(ad.s.mplays);
		splays->SetText(ad.s.splays);
		audioState->InsertEndChild(mkey);
		audioState->InsertEndChild(skey);
		audioState->InsertEndChild(mplays);
		audioState->InsertEndChild(splays);

		audio->InsertEndChild(audioList);
		audio->InsertEndChild(audioState);
		root->InsertEndChild(audio);
	}

	void dealWithGraphic(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root, Artist* partist) {
		using namespace tinyxml2;

		Artist::dump dd = partist->Dumb();
		XMLElement* graphic = doc.NewElement("Graphic");

		XMLElement* textureList = doc.NewElement("List");
		for (int i = 0; i < dd.list.size(); ++i) {
			XMLElement* listelem = doc.NewElement("a");
			XMLElement* f = doc.NewElement("filename");
			XMLElement* k = doc.NewElement("key");
			XMLElement* r = doc.NewElement("rows");
			XMLElement* c = doc.NewElement("collumns");
			f->SetText(dd.list[i].filename.c_str());
			k->SetText(dd.list[i].key.c_str());
			r->SetText(dd.list[i].r);
			c->SetText(dd.list[i].c);
			listelem->InsertEndChild(f);
			listelem->InsertEndChild(k);
			listelem->InsertEndChild(r);
			listelem->InsertEndChild(c);
			textureList->InsertEndChild(listelem);
		}
		graphic->InsertEndChild(textureList);

		XMLElement* screen = doc.NewElement("Screen");
		XMLElement* backColor = doc.NewElement("backgroundColor");
		XMLElement* textureKey = doc.NewElement("textureKey");
		XMLElement* drawBackPic = doc.NewElement("drawBackPic");
		XMLElement* colorChanged = doc.NewElement("colorChanged");
		XMLElement* dest = doc.NewElement("dest");
		XMLElement* stretchState = doc.NewElement("stretchState");
		XMLElement* q = doc.NewElement("q");
		XMLElement* id = doc.NewElement("id");

		vectoxml(doc, backColor, dd.s.backgroundColor);
		textureKey->SetText(dd.s.textureKey.c_str());
		drawBackPic->SetText(dd.s.drawBackPic);
		colorChanged->SetText(dd.s.colorChanged);
		vectoxml(doc, dest, dd.s.dest);
		stretchState->SetText(dd.s.stretchState);
		id->SetText(dd.s.id);

		for (const auto& pair : dd.s.q) {
			XMLElement* le = doc.NewElement("a");
			XMLElement* id = doc.NewElement("id");
			XMLElement* tk = doc.NewElement("textureKey");
			XMLElement* s = doc.NewElement("source");
			XMLElement* d = doc.NewElement("dest");
			id->SetText(pair.first);
			tk->SetText(pair.second.texture->key.c_str());
			vectoxml(doc, s, pair.second.source);
			vectoxml(doc, d, pair.second.destination);
			le->InsertEndChild(id);
			le->InsertEndChild(tk);
			le->InsertEndChild(s);
			le->InsertEndChild(d);
			q->InsertEndChild(le);
		}

		screen->InsertEndChild(backColor);
		screen->InsertEndChild(textureKey);
		screen->InsertEndChild(drawBackPic);
		screen->InsertEndChild(colorChanged);
		screen->InsertEndChild(dest);
		screen->InsertEndChild(stretchState);
		screen->InsertEndChild(q);
		screen->InsertEndChild(id);
		graphic->InsertEndChild(screen);

		root->InsertEndChild(graphic);
	}

	void dealWithWidgets(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) {
		using namespace tinyxml2;

		WidgetsManager::dump wd = WM_INSTANCE.Dump();
		XMLElement* widgets = doc.NewElement("Widgets");

		XMLElement* ts = doc.NewElement("Texts");
		XMLElement* bs = doc.NewElement("Buttons");
		XMLElement* tbs = doc.NewElement("Textboxs");

		for (const auto& t : wd.ts) {
			XMLElement* le = doc.NewElement("a");
			XMLElement* name = doc.NewElement("name");
			XMLElement* geometry = doc.NewElement("geometry");
			XMLElement* text = doc.NewElement("text");
			XMLElement* font = doc.NewElement("font");
			XMLElement* backimage= doc.NewElement("backimage");
			XMLElement* textcolor= doc.NewElement("textcolor");
			XMLElement* backcolor= doc.NewElement("backcolor");
			XMLElement* bordercolor= doc.NewElement("bordercolor");
			XMLElement* align= doc.NewElement("align");
			XMLElement* shown= doc.NewElement("shown");
			XMLElement* showborder= doc.NewElement("showborder");
			XMLElement* backisswown= doc.NewElement("backisswown");
			XMLElement* wrapped= doc.NewElement("wrapped");
			XMLElement* hindent= doc.NewElement("hindent");
			XMLElement* vindent= doc.NewElement("vindent");

			name->SetText(t.first.c_str());
			vectoxml(doc, geometry, t.second.geometry);
			text->SetText(t.second.text.c_str());
			font->SetText(t.second.font.c_str());
			backimage->SetText(t.second.backimage.c_str());
			vectoxml(doc, textcolor, t.second.textcolor);
			vectoxml(doc, backcolor, t.second.backcolor);
			vectoxml(doc, bordercolor, t.second.bordercolor);
			align->SetText(t.second.align);
			shown->SetText(t.second.shown);
			showborder->SetText(t.second.showborder);
			backisswown->SetText(t.second.backisshown);
			wrapped->SetText(t.second.wrapped);
			hindent->SetText(t.second.hindent);
			vindent->SetText(t.second.vindent);

			le->InsertEndChild(name);
			le->InsertEndChild(geometry);
			le->InsertEndChild(text);
			le->InsertEndChild(font);
			le->InsertEndChild(backimage);
			le->InsertEndChild(textcolor);
			le->InsertEndChild(backcolor);
			le->InsertEndChild(bordercolor);
			le->InsertEndChild(align);
			le->InsertEndChild(shown);
			le->InsertEndChild(showborder);
			le->InsertEndChild(backisswown);
			le->InsertEndChild(wrapped);
			le->InsertEndChild(hindent);
			le->InsertEndChild(vindent);
			ts->InsertEndChild(le);
		}

		for (const auto& t : wd.bs) {
			XMLElement* le = doc.NewElement("a");
			XMLElement* name = doc.NewElement("name");
			XMLElement* geometry = doc.NewElement("geometry");
			XMLElement* text = doc.NewElement("text");
			XMLElement* font = doc.NewElement("font");
			XMLElement* backimage = doc.NewElement("backimage");
			XMLElement* textcolor = doc.NewElement("textcolor");
			XMLElement* backcolor = doc.NewElement("backcolor");
			XMLElement* bordercolor = doc.NewElement("bordercolor");
			XMLElement* align = doc.NewElement("align");
			XMLElement* shown = doc.NewElement("shown");
			XMLElement* showborder = doc.NewElement("showborder");
			XMLElement* backisswown = doc.NewElement("backisswown");
			XMLElement* wrapped = doc.NewElement("wrapped");
			XMLElement* hindent = doc.NewElement("hindent");
			XMLElement* vindent = doc.NewElement("vindent");
			XMLElement* defaultborder = doc.NewElement("defaultborder");
			XMLElement* focusborder = doc.NewElement("focusborder");

			name->SetText(t.first.c_str());
			vectoxml(doc, geometry, t.second.ts.geometry);
			text->SetText(t.second.ts.text.c_str());
			font->SetText(t.second.ts.font.c_str());
			backimage->SetText(t.second.ts.backimage.c_str());
			vectoxml(doc, textcolor, t.second.ts.textcolor);
			vectoxml(doc, backcolor, t.second.ts.backcolor);
			vectoxml(doc, bordercolor, t.second.ts.bordercolor);
			align->SetText(t.second.ts.align);
			shown->SetText(t.second.ts.shown);
			showborder->SetText(t.second.ts.showborder);
			backisswown->SetText(t.second.ts.backisshown);
			wrapped->SetText(t.second.ts.wrapped);
			hindent->SetText(t.second.ts.hindent);
			vindent->SetText(t.second.ts.vindent);
			vectoxml(doc, defaultborder, t.second.defaultborder);
			vectoxml(doc, focusborder, t.second.focusborder);

			le->InsertEndChild(name);
			le->InsertEndChild(geometry);
			le->InsertEndChild(text);
			le->InsertEndChild(font);
			le->InsertEndChild(backimage);
			le->InsertEndChild(textcolor);
			le->InsertEndChild(backcolor);
			le->InsertEndChild(bordercolor);
			le->InsertEndChild(align);
			le->InsertEndChild(shown);
			le->InsertEndChild(showborder);
			le->InsertEndChild(backisswown);
			le->InsertEndChild(wrapped);
			le->InsertEndChild(hindent);
			le->InsertEndChild(vindent);
			le->InsertEndChild(defaultborder);
			le->InsertEndChild(focusborder);
			ts->InsertEndChild(le);
		}

		for (const auto& t : wd.tbs) {
			XMLElement* le = doc.NewElement("a");
			XMLElement* name = doc.NewElement("name");
			XMLElement* geometry = doc.NewElement("geometry");
			XMLElement* text = doc.NewElement("text");
			XMLElement* font = doc.NewElement("font");
			XMLElement* backimage = doc.NewElement("backimage");
			XMLElement* textcolor = doc.NewElement("textcolor");
			XMLElement* backcolor = doc.NewElement("backcolor");
			XMLElement* bordercolor = doc.NewElement("bordercolor");
			XMLElement* align = doc.NewElement("align");
			XMLElement* shown = doc.NewElement("shown");
			XMLElement* showborder = doc.NewElement("showborder");
			XMLElement* backisswown = doc.NewElement("backisswown");
			XMLElement* wrapped = doc.NewElement("wrapped");
			XMLElement* hindent = doc.NewElement("hindent");
			XMLElement* vindent = doc.NewElement("vindent");
			XMLElement* currentstring = doc.NewElement("currentstring");
			XMLElement* maxchar = doc.NewElement("maxchar");

			name->SetText(t.first.c_str());
			vectoxml(doc, geometry, t.second.ts.geometry);
			text->SetText(t.second.ts.text.c_str());
			font->SetText(t.second.ts.font.c_str());
			backimage->SetText(t.second.ts.backimage.c_str());
			vectoxml(doc, textcolor, t.second.ts.textcolor);
			vectoxml(doc, backcolor, t.second.ts.backcolor);
			vectoxml(doc, bordercolor, t.second.ts.bordercolor);
			align->SetText(t.second.ts.align);
			shown->SetText(t.second.ts.shown);
			showborder->SetText(t.second.ts.showborder);
			backisswown->SetText(t.second.ts.backisshown);
			wrapped->SetText(t.second.ts.wrapped);
			hindent->SetText(t.second.ts.hindent);
			vindent->SetText(t.second.ts.vindent);
			currentstring->SetText(t.second.currentString.c_str());
			maxchar->SetText(t.second.maxchar);

			le->InsertEndChild(name);
			le->InsertEndChild(geometry);
			le->InsertEndChild(text);
			le->InsertEndChild(font);
			le->InsertEndChild(backimage);
			le->InsertEndChild(textcolor);
			le->InsertEndChild(backcolor);
			le->InsertEndChild(bordercolor);
			le->InsertEndChild(align);
			le->InsertEndChild(shown);
			le->InsertEndChild(showborder);
			le->InsertEndChild(backisswown);
			le->InsertEndChild(wrapped);
			le->InsertEndChild(hindent);
			le->InsertEndChild(vindent);
			le->InsertEndChild(currentstring);
			le->InsertEndChild(maxchar);
			ts->InsertEndChild(le);
		}

		widgets->InsertEndChild(ts);
		widgets->InsertEndChild(bs);
		widgets->InsertEndChild(tbs);
		root->InsertEndChild(widgets);
	}

	void SaveLoad::Save(int number, Artist* partist) {
		using namespace tinyxml2;

		if (!SM_INSTANCE.isThereAReading()) return;

		if (!(std::filesystem::exists(sSaveDir)))
			std::filesystem::create_directory(sSaveDir);

		XMLDocument doc;
		XMLDeclaration* dec = doc.NewDeclaration();
		doc.InsertFirstChild(dec);
		XMLElement* root = doc.NewElement("VNEngine_Save");

		size_t sp = (size_t)SM_INSTANCE.GetTopReaderPos();

		dealWithAudio(doc, root);
		dealWithGraphic(doc,root,partist);
		dealWithWidgets(doc, root);

		XMLElement* readingpos = doc.NewElement("ReadingPos");
		readingpos->SetText(sp);
		root->InsertEndChild(readingpos);
		
		doc.InsertEndChild(root);
		doc.SaveFile((sSaveDir + std::to_string(number) + ".vns").c_str());

		auto s = partist->GetWindowSize();
		if (!(std::filesystem::exists(TM_INSTANCE.GetImagesPath() + "savescreens")))
			std::filesystem::create_directory(TM_INSTANCE.GetImagesPath() + "savescreens");
		auto savescreen = partist->GetScreenshot({s.x/4,s.y/4});
		IMG_SavePNG(savescreen, (TM_INSTANCE.GetImagesPath() + "savescreens/" + std::to_string(number) + ".png").c_str());
	}
	
	void undealWithAudio(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) {
		using namespace tinyxml2;

		AudioPlayer::dump d = {};
		XMLElement* audio = root->FirstChildElement("Audio");

		XMLElement* list = audio->FirstChildElement("List");
		for (XMLElement* le = list->FirstChildElement("a"); le; le = le->NextSiblingElement("a")) {
			XMLElement* f = le->FirstChildElement("filename");
			XMLElement* k = le->FirstChildElement("key");
			AP_INSTANCE.AddAudio(f->GetText(), k->GetText());
		}

		std::string m, s;
		XMLElement* state = audio->FirstChildElement("State");
		auto xm = state->FirstChildElement("mkey");
		auto xs = state->FirstChildElement("skey");
		xm->GetText() ? m = xm->GetText() : m = "";
		xs->GetText() ? s = xs->GetText() : s = "";
		XMLElement* mp = state->FirstChildElement("mplays");
		XMLElement* sp = state->FirstChildElement("splays");
		
		bool musicplays = std::string(mp->GetText()) == "true";
		bool soundplays = std::string(sp->GetText()) == "true";

		bool notmuten = !(AP_INSTANCE.GetIfMute());
		if (notmuten) AP_INSTANCE.Mute();
		if (!m.empty()) AP_INSTANCE.PlayMusic(m);
		if (!s.empty()) AP_INSTANCE.PlaySound(s);
		if (!musicplays) AP_INSTANCE.StopMusic();
		if (!soundplays) AP_INSTANCE.StopSound();
		if (notmuten) AP_INSTANCE.Unmute();
	}

	void undealWithGraphic(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root, Artist* partist) {
		using namespace tinyxml2;

		Artist::dump dd = {};
		XMLElement* graphic = root->FirstChildElement("Graphic");
		XMLElement* list = graphic->FirstChildElement("List");
		XMLElement* screen = graphic->FirstChildElement("Screen");

		for (XMLElement* le = list->FirstChildElement("a"); le; le = le->NextSiblingElement("a")) {
			TM_INSTANCE.addTexture(
				le->FirstChildElement("key")->GetText(),
				le->FirstChildElement("filename")->GetText(),
				atoi(le->FirstChildElement("rows")->GetText()),
				atoi(le->FirstChildElement("collumns")->GetText())
			);
		}

		std::unordered_map<uint32_t, DrawnData> q;
		XMLElement* queue = screen->FirstChildElement("q");
		for (XMLElement* le = queue->FirstChildElement("a"); le; le = le->NextSiblingElement("a")) {
			q[(atoi(le->FirstChildElement("id")->GetText()))] = {
				TM_INSTANCE.getTexture(le->FirstChildElement("textureKey")->GetText()),
				xmltovec(le->FirstChildElement("source")),
				xmltovec(le->FirstChildElement("dest"))
			};
		}

		Artist::screen s = {
			xmltouvec(screen->FirstChildElement("backgroundColor")),
			screen->FirstChildElement("textureKey")->GetText(),
			std::string(screen->FirstChildElement("drawBackPic")->GetText()) == "true",
			std::string(screen->FirstChildElement("colorChanged")->GetText()) == "true",
			xmltovec(screen->FirstChildElement("dest")),
			(Stretching)(uint8_t)atoi(screen->FirstChildElement("stretchState")->GetText()),
			q,
			(uint32_t)atoi(screen->FirstChildElement("id")->GetText())
		};
		partist->Load(s);
	}
	
	void undealWithWidgets(tinyxml2::XMLDocument& doc, tinyxml2::XMLElement* root) {
		using namespace tinyxml2;

		WidgetsManager::dump wd = {};
		XMLElement* widgets = root->FirstChildElement("Widgets");
		XMLElement* ts = widgets->FirstChildElement("Texts");
		XMLElement* bs = widgets->FirstChildElement("Buttons");
		XMLElement* tbs = widgets->FirstChildElement("Textboxs");

		for (XMLElement* w = ts->FirstChildElement("a"); w; w = w->NextSiblingElement("a")) {
			XMLElement* name = w->FirstChildElement("name");
			XMLElement* geometry = w->FirstChildElement("geometry");
			XMLElement* text = w->FirstChildElement("text");
			XMLElement* font = w->FirstChildElement("font");
			XMLElement* backimage = w->FirstChildElement("backimage");
			XMLElement* textcolor = w->FirstChildElement("textcolor");
			XMLElement* backcolor = w->FirstChildElement("backcolor");
			XMLElement* bordercolor = w->FirstChildElement("bordercolor");
			XMLElement* align = w->FirstChildElement("align");
			XMLElement* shown = w->FirstChildElement("shown");
			XMLElement* showborder = w->FirstChildElement("showborder");
			XMLElement* backisswown = w->FirstChildElement("backisswown");
			XMLElement* wrapped = w->FirstChildElement("wrapped");
			XMLElement* hindent = w->FirstChildElement("hindent");
			XMLElement* vindent = w->FirstChildElement("vindent");

			textState t = {
				xmltovec(geometry),
				text->GetText() ? text->GetText() : std::string(),
				font->GetText() ? font->GetText() : std::string(),
				backimage->GetText() ? backimage->GetText() : std::string(),
				xmltouvec(textcolor), xmltouvec(backcolor),xmltouvec(bordercolor),
				(uint8_t)atoi(align->GetText()),
				std::string(shown->GetText()) == "true" ? true : false,
				std::string(showborder->GetText()) == "true" ? true : false,
				std::string(backisswown->GetText()) == "true" ? true : false,
				std::string(wrapped->GetText()) == "true" ? true : false,
				atoi(hindent->GetText()), atoi(vindent->GetText())
			};
			wd.ts.push_back({name->GetText(),t});
		}
		for (XMLElement* w = bs->FirstChildElement("a"); w; w = w->NextSiblingElement("a")) {
			XMLElement* name = w->FirstChildElement("name");
			XMLElement* geometry = w->FirstChildElement("geometry");
			XMLElement* text = w->FirstChildElement("text");
			XMLElement* font = w->FirstChildElement("font");
			XMLElement* backimage = w->FirstChildElement("backimage");
			XMLElement* textcolor = w->FirstChildElement("textcolor");
			XMLElement* backcolor = w->FirstChildElement("backcolor");
			XMLElement* bordercolor = w->FirstChildElement("bordercolor");
			XMLElement* align = w->FirstChildElement("align");
			XMLElement* shown = w->FirstChildElement("shown");
			XMLElement* showborder = w->FirstChildElement("showborder");
			XMLElement* backisswown = w->FirstChildElement("backisswown");
			XMLElement* wrapped = w->FirstChildElement("wrapped");
			XMLElement* hindent = w->FirstChildElement("hindent");
			XMLElement* vindent = w->FirstChildElement("vindent");
			XMLElement* defaultborder = w->FirstChildElement("defaultborder");
			XMLElement* focusborder = w->FirstChildElement("focusborder");

			textState t = {
				xmltovec(geometry),
				text->GetText() ? text->GetText() : std::string(),
				font->GetText() ? font->GetText() : std::string(),
				backimage->GetText() ? backimage->GetText() : std::string(),
				xmltouvec(textcolor), xmltouvec(backcolor),xmltouvec(bordercolor),
				(uint8_t)atoi(align->GetText()),
				std::string(shown->GetText()) == "true" ? true : false,
				std::string(showborder->GetText()) == "true" ? true : false,
				std::string(backisswown->GetText()) == "true" ? true : false,
				std::string(wrapped->GetText()) == "true" ? true : false,
				atoi(hindent->GetText()), atoi(vindent->GetText())
			};
			buttonState b = {
				t, xmltouvec(defaultborder),xmltouvec(focusborder), nullptr
			};
			wd.bs.push_back({ name->GetText(),b });
		}
		for (XMLElement* w = tbs->FirstChildElement("a"); w; w = w->NextSiblingElement("a")) {
			XMLElement* name = w->FirstChildElement("name");
			XMLElement* geometry = w->FirstChildElement("geometry");
			XMLElement* text = w->FirstChildElement("text");
			XMLElement* font = w->FirstChildElement("font");
			XMLElement* backimage = w->FirstChildElement("backimage");
			XMLElement* textcolor = w->FirstChildElement("textcolor");
			XMLElement* backcolor = w->FirstChildElement("backcolor");
			XMLElement* bordercolor = w->FirstChildElement("bordercolor");
			XMLElement* align = w->FirstChildElement("align");
			XMLElement* shown = w->FirstChildElement("shown");
			XMLElement* showborder = w->FirstChildElement("showborder");
			XMLElement* backisswown = w->FirstChildElement("backisswown");
			XMLElement* wrapped = w->FirstChildElement("wrapped");
			XMLElement* hindent = w->FirstChildElement("hindent");
			XMLElement* vindent = w->FirstChildElement("vindent");
			XMLElement* currentstring = w->FirstChildElement("currentstring");
			XMLElement* maxchar = w->FirstChildElement("maxchar");

			textState t = {
				xmltovec(geometry),
				text->GetText() ? text->GetText() : std::string(),
				font->GetText() ? font->GetText() : std::string(),
				backimage->GetText() ? backimage->GetText() : std::string(),
				xmltouvec(textcolor), xmltouvec(backcolor),xmltouvec(bordercolor),
				(uint8_t)atoi(align->GetText()),
				std::string(shown->GetText()) == "true" ? true : false,
				std::string(showborder->GetText()) == "true" ? true : false,
				std::string(backisswown->GetText()) == "true" ? true : false,
				std::string(wrapped->GetText()) == "true" ? true : false,
				atoi(hindent->GetText()), atoi(vindent->GetText())
			};
			textboxState tb = {
				t, currentstring->GetText(), (uint32_t)atoi(maxchar->GetText())
			};
			wd.tbs.push_back({ name->GetText(),tb });
		}

		WM_INSTANCE.Load(wd);
	}

	int SaveLoad::Load(int number, Artist* partist) {
		using namespace tinyxml2;

		if (!(std::filesystem::exists(sSaveDir + std::to_string(number) + ".vns"))) return -1;

		XMLDocument doc;
		doc.LoadFile((sSaveDir + std::to_string(number) + ".vns").c_str());
		XMLElement* root = doc.RootElement();

		undealWithAudio(doc, root);
		undealWithGraphic(doc, root, partist);
		undealWithWidgets(doc, root);

		std::string sps = root->FirstChildElement("ReadingPos")->GetText();
		int sp = atoi(sps.c_str());

		return sp;
	}
	
	void SaveLoad::SetSaveDir(const std::string& path) {
		sSaveDir = path;
	}
	
	std::string SaveLoad::GetSaveDir() {
		return sSaveDir;
	}
}