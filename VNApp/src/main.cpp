#include <VNEngine/Core/App.hpp>

class Game : public VNEngine::App {
public:
    Game() : VNEngine::App() {}

private:
};

int main(int argc, char** args) {
    (void)argc;
    (void)args;

    Game game = Game();
    game.Run();

    return 0;
}