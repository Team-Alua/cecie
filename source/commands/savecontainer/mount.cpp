#include "log.hpp"
#include "commands/savecontainer/mount.hpp"
#include <string.h>
#include "orbis/SaveData.h"
#include "orbis/UserService.h"

void MountSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);

	if (strlen(clientSession->mountPath) > 0) {
		network.sendResponse("ok");
		return;
	}

	OrbisSaveDataMount mount;
	memset(&mount, 0, sizeof(OrbisSaveDataMount));
	mount.titleId = clientSession->titleId;
	mount.dirName = clientSession->dirName;

	char fingerprint[80];
	memset(fingerprint, 0, sizeof(fingerprint));
	strcpy(fingerprint, "0000000000000000000000000000000000000000000000000000000000000000");
	mount.fingerprint = fingerprint;

	mount.blocks = clientSession->saveBlocks;

	int32_t outUserId;
	int32_t initialUserResult = sceUserServiceGetInitialUser(&outUserId);
	if (initialUserResult < 0) {
		char msg[64];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "sony.error.%X", initialUserResult);
		network.sendResponse(msg);
		return;
	}

	mount.userId = outUserId;

	mount.mountMode = ORBIS_SAVE_DATA_MOUNT_MODE_CREATE2;
	mount.mountMode |= ORBIS_SAVE_DATA_MOUNT_MODE_RDWR;
	mount.mountMode |= ORBIS_SAVE_DATA_MOUNT_MODE_DESTRUCT_OFF;

	log("titleId (size): %d dirName (size): %d", sizeof(clientSession->titleId), sizeof(clientSession->dirName));
	log("titleId: %s dirName: %s blocks: %X userId: %X", mount.titleId, mount.dirName, mount.blocks, mount.userId);

	OrbisSaveDataMountResult mountResult;
	memset(&mountResult, 0, sizeof(mountResult));
	int32_t result = sceSaveDataMount(&mount, &mountResult);
	if (result < 0) {
		char msg[64];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "sony.error.%X", result);
		network.sendResponse(msg);
	} else {
		memset(clientSession->mountPath, 0, sizeof(clientSession->mountPath));
		strcpy(clientSession->mountPath, mountResult.mountPathName);
		network.sendResponse("ok");
	}
}

