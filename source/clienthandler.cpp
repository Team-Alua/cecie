#include <unistd.h>

#include "clienthandler.hpp"
#include "log.hpp"
#include "network.hpp"
#include "commands/system.hpp"

#include "orbis/SaveData.h"

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
		log("Command Set %s is active.", setPtr->Name());
		char commandName[32];
		auto result = network.readFull(&commandName); 
		if (result == 0) {
			log("Client disconnected.");
			commandSystem.MarkFinished();
			continue;
		} else if (result < 0) {
			log("There was an error %d.", errno);
			commandSystem.MarkFinished();
			continue;
		}
		auto cmd = setPtr->Get(commandName);
		if (cmd == NULL) {
			log("User supplied invalid command %s.", commandName);
			break;
		}
		network.sendResponse("ok");
		log("Executing cmd %s.", commandName);
		cmd->Execute(network, sessionIndex, *sessions);
	}

	ClientSession * clientSession = sessions->Get(sessionIndex);

	if (clientSession != NULL && strlen(clientSession->mountPath) > 0) {
		OrbisSaveDataMountPoint mp;
		memset(&mp, 0, sizeof(mp));
		strcpy(mp.data, clientSession->mountPath);
		sceSaveDataUmount(&mp);
	}

	if (sessionIndex >= 0) {
		
		log("Freeing %i", sessionIndex);
		sessions->Free(sessionIndex);
	}

	close(connfd);
}

