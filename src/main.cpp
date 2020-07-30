#include <iostream>
#include "Engine.h"
#include <ctime>
int main(int argc, char *argv[]) {
	srand((unsigned int)time(NULL));
	Engine engine;
	engine.Init();
	engine.Run();
	return 0;
}