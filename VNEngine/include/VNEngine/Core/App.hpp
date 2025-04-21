#pragma once

#include <VNEngine/Core/export.hpp>
#include <VNEngine/Graphic/Artist.hpp>
#include <VNEngine/LuaReaders/Loader.hpp>

namespace VNEngine {

class VN_ENGINE_API App {
    friend Loader;

public:
    App();
    virtual ~App();

    void run();

protected:
    std::unique_ptr<Artist> drawer_;

    virtual void load();

    virtual void update();
    void handleEvents();
    void draw();

    bool isRunning_;
};
} // namespace VNEngine