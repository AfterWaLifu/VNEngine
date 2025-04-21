#include <VNEngine/Core/App.hpp>

#include <VNEngine/Controls/InputHandler.hpp>
#include <VNEngine/StateMachine/MenuState.hpp>
#include <VNEngine/StateMachine/StateMachine.hpp>
#include <VNEngine/Widgets/WidgetsManager.hpp>

namespace VNEngine {

App::App() : drawer_(nullptr), isRunning_(false) {
    Loader l = Loader(this);
    l.Load();

    isRunning_ = true;

    load();
}

App::~App() {
    Loader l(this);
    l.Save();
}

void App::run() {
    uint64_t ticks, diff;
    while (isRunning_) {
        ticks = SDL_GetTicks64();
        update();
        handleEvents();
        draw();
        diff = ticks = SDL_GetTicks64();
        if (diff < 16)
            SDL_Delay((uint32_t)(16 - diff));
    }
}

void App::load() { SM_INSTANCE.ChangeState(new MenuState("mainmenu")); }

void App::update() {}

void App::handleEvents() {
    IH_INSTANCE.Update();
    WM_INSTANCE.Handle();
    SM_INSTANCE.Update();
}

void App::draw() {
    drawer_->Perform();
    SM_INSTANCE.Draw();
    WM_INSTANCE.Draw();
    drawer_->DrawAkaFinale();
}
} // namespace VNEngine