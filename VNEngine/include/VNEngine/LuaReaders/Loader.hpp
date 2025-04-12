#pragma once

#include <VNEngine/LuaReaders/LuaReader.hpp>

namespace VNEngine {

class App;

class Loader : public LuaReader {
    App* m_pApp;

    void readPresets();
    void readSettings();
    void writeSettings();
    void preload();

public:
    Loader(App* app);
    ~Loader();

    void Load();
    void Save();
};
} // namespace VNEngine