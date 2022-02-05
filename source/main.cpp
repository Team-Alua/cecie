#include "main.hpp"

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	
	std::thread pongThread(pongServer);
	pongThread.detach();

	std::thread cmdServerThread(cmdServer);
	cmdServerThread.detach();

	INF_LOOP
}

