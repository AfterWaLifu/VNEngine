#pragma once

#include "LuaReaders/LuaReader.h"

namespace VNEngine {

	class App;

	class Loader : public LuaReader {



	public:
		Loader(App* app);
		~Loader();
	};
}