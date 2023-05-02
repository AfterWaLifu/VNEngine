#pragma once

#include "LuaReaders/LuaReader.h"

#include <string>

namespace VNEngine {

	class InterfaceCreator : public LuaReader {

	public:
		InterfaceCreator();
		~InterfaceCreator();

		void Draw(const std::string& what);
	};
}