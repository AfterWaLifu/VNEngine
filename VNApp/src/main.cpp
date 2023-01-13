#include <iostream>

#include "VNEngine.h"

int main(int argc, char* argv[]) {
	
	VNEngine::Window* window = new VNEngine::Window();
	window->Run(3000);
	delete window;

	return 0;
}