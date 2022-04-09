#include <unistd.h>

#include "clienthandler.hpp"
#include "log.hpp"
#include "network.hpp"
#include "commands/system.hpp"

void clientHandler(int connfd, Sessions * sessions) {
	
	int sessionIndex = -1;

	CommandSystem commandSystem;
	commandSystem.Initialize();

	Network network(connfd);
	while(!commandSystem.Finished()) {
		auto setPtr = commandSystem.GetCommandSet();
		if (setPtr == NULL) {
			log("No command set was found. Terminating loop.");
			break;
		}
		char commandName[32];
		if (network.readFull(&commandName) <= 0) {
			log("Error occured or client disconnected.");
			commandSystem.MarkFinished();
			continue;
		}
		auto cmd = setPtr->Get(commandName);
		
		if (cmd == NULL) {
			log("User supplied invalid command %s.", commandName);
			network.sendResponse("invalid.cmd");
			continue;
		}
		cmd->Execute(network, sessionIndex, *sessions);
	}
	if (sessionIndex >= 0) {
		// do cleanup
	}
	close(connfd);
}

