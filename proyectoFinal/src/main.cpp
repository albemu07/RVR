#include <iostream>

#include "game/Game.h"
#include <thread>

int main(int ac, char **av) {

	Game e = Game(av[1], av[2], av[3]);
	try {
		e.init();
		std::thread net_thread([&e](){ e.exitLoop(); });
		e.gameLoop();
		net_thread.detach();
		e.didIWin();
	} catch (const std::string &e) { // catch exceptions thrown as strings
		std::cerr << e << std::endl;
	} catch (const char *e) { // catch exceptions thrown as char*
		std::cerr << e << std::endl;
	} catch (const std::exception &e) { // catch exceptions thrown as a sub-type of std::exception
		std::cerr << e.what();
	} catch (...) {
		std::cerr << "Caught and exception of unknown type ...";
	}

	return 0;
}
