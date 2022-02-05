#include "main.hpp"

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	
	std::thread pongThread(pongServer);
	pongThread.detach();
	INF_LOOP
}

