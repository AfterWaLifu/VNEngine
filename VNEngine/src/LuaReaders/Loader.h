#pragma once

#include "LuaReaders/LuaReader.h"

namespace VNEngine {

	class App;

	class Loader : public LuaReader {
		App* m_pApp;

		void readPresets();
		void readSettings();
		void preload();

	public:
		Loader(App* app);
		~Loader();
	};
}