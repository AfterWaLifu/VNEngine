#pragma once

#include <string>

namespace VNEngine {
	class Artist;

	class SaveLoad {
		static inline std::string sSaveDir;
	public:
		static void Save(int number, Artist* partist);
		static int Load(int number, Artist* partist);

		static void SetSaveDir(const std::string& path);
		static std::string GetSaveDir();
	};
}