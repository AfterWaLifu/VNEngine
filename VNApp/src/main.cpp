#include <iostream>

#include "VNEngine.h"

class Game : public VNEngine::App {
public:
	Game() : VNEngine::App() {  }
private:
};

int main(int argc, char* argv[]) {
	
	Game game = Game();
	game.Run();

	return 0;
}