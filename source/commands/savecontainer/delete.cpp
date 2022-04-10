#include "commands/savecontainer/delete.hpp"

#include "orbis/UserService.h"
#include "orbis/SaveData.h"

void DeleteSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) > 0) {
		network.sendResponse("delete.mounted");
		return;
	}
	int32_t outUserId;
	int32_t initialUserResult = sceUserServiceGetInitialUser(&outUserId);
	if (initialUserResult < 0) {
		char msg[32];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "sony.error.%X", initialUserResult);
		network.sendResponse(msg);
		return;
	}


	OrbisSaveDataDelete del;
	memset(&del, 0, sizeof(OrbisSaveDataDelete));

	del.userId = outUserId;
	del.dirName = (OrbisSaveDataDirName*) &clientSession->dirName;
	del.titleId = (OrbisSaveDataTitleId*) &clientSession->titleId;
	
	int32_t result = sceSaveDataDelete(&del);
	if (result < 0) {
		char msg[32];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "sony.error.%X", result);
		network.sendResponse(msg);
		return;
	}
	network.sendResponse("ok");
}
