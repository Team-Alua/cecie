#include "commands/savecontainer/unmount.hpp"

#include "orbis/SaveData.h"

void UnmountSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	
	if (strlen(clientSession->mountPath) == 0) {
		network.sendResponse("ok");
		return;
	}

	OrbisSaveDataMountPoint mp;
	memset(&mp, 0, sizeof(mp));
	strcpy(mp.data, clientSession->mountPath);
	int32_t result = sceSaveDataUmount(&mp);

	if (result < 0) {
		char msg[64];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "sony.error.%X", result);
		network.sendResponse(msg);
	} else {
		// Reset mount path
		memset(clientSession->mountPath, 0, sizeof(clientSession->mountPath));
		network.sendResponse("ok");
	}
}
