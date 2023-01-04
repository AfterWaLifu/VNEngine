#include <iostream>

#include "VNEngine.h"

int main() {
	VNEngine::Logger logger;
	std::cout << "Logger working? " << std::endl;
	logger.AreYouWorking();
	return 0;
}