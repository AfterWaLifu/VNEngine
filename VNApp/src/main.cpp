#include <VNEngine/Core/App.hpp>

int main(int argc, char** args) {
    (void)argc;
    (void)args;

    auto game = VNEngine::App();
    game.run();

    return 0;
}