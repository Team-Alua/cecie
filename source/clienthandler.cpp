#include <unistd.h>

#include "clienthandler.hpp"
#include "log.hpp"

#include "commands/system.hpp"

void clientHandler(int connfd, Sessions * sessions) {
	
	int sessionIndex = -1;

	CommandSystem commandSystem;
	commandSystem.Initialize();

	while(!commandSystem.Finished()) {
		auto setPtr = commandSystem.GetCommandSet();
		if (setPtr == NULL) {
			log("No command set was found. Terminating loop.");
			break;
		}

		// Read Command Name From Network
		auto cmd = setPtr->Get("abc");
		cmd->Execute(connfd, sessionIndex, *sessions);
	}
	close(connfd);
}

