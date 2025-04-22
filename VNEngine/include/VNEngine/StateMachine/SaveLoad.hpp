#pragma once

#include <string>

struct lua_State;

namespace VNEngine {

class Artist;

class SaveLoad {
public:
    static void save(int number, Artist* partist, lua_State* L);
    static std::size_t load(int number, Artist* partist, lua_State* L);

    static void setSaveDir(const std::string& path);
    static std::string getSaveDir();

private:
    static inline std::string sSaveDir;
};

} // namespace VNEngine