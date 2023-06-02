#pragma once

#include <string>

struct lua_State;

namespace VNEngine {
	class Artist;

	class SaveLoad {
		static inline std::string sSaveDir;
	public:
		static void Save(int number, Artist* partist, lua_State* L);
		static size_t Load(int number, Artist* partist, lua_State* L);

		static void SetSaveDir(const std::string& path);
		static std::string GetSaveDir();
	};
}