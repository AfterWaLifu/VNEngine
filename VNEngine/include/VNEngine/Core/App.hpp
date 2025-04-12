#pragma once

#include <VNEngine/Audio/AudioPlayer.hpp>
#include <VNEngine/Controls/InputHandler.hpp>
#include <VNEngine/Core/Logger.hpp>
#include <VNEngine/Core/export.hpp>
#include <VNEngine/Graphic/Artist.hpp>
#include <VNEngine/LuaReaders/Loader.hpp>
#include <VNEngine/StateMachine/MenuState.hpp>
#include <VNEngine/StateMachine/ReadingState.hpp>
#include <VNEngine/StateMachine/StateMachine.hpp>
#include <VNEngine/Widgets/WidgetsManager.hpp>

namespace VNEngine {

/**
 * @brief Main app class
 * Being used to use/inherint and change smth if you need
 */
class VN_ENGINE_API App {
    friend Loader;

protected:
    std::unique_ptr<Artist> m_Drawer; /// Artist class object, works with graphics

    virtual void Load(); /// Function for preparing for work

    virtual void Update(); /// Func to change smth you need and when you need
    void HandleEvents();   /// Handles events (keys, mouse etc)
    void Draw();           /// Calling drawing of everything in order

    bool m_IsRunning; /// State of the app, if it should be closed (autochanged)

public:
    /**
     * @brief App constructor, creates App
     * @param	defaultAppInfo	Window default params
     */
    App();
    virtual ~App(); /// App destructor

    void Run(); /// Function that starts work of the app
};
} // namespace VNEngine