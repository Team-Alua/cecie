#include "commands/savecontainer/upload.hpp"


#include "orbis/UserService.h"

void UploadSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) > 0) {
		network.sendResponse("upload.mounted");
		return;
	}

	int32_t outUserId;
	int32_t initUserResult = sceUserServiceGetInitialUser(&outUserId);
	if (initUserResult < 0) {
		char msg[32];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "sony.error.%X", initUserResult);
		network.sendResponse(msg);
		return;
	}

	char targetFile[256];
	memset(targetFile, 0, sizeof(targetFile));

	sprintf(targetFile, "/user/home/%x/savedata/%s/%s.bin", outUserId, clientSession->titleId, clientSession->dirName);

	int result = open(targetFile, O_EXCL | O_CREAT);
	if (result == -1 && errno != EEXIST) {
		char msg[32];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "upload.open.error.%i", errno);
		network.sendResponse(msg);
		return;
	}

	if (result != -1) {
		close(result);
		remove(targetFile);
		network.sendResponse("upload.notexist");
		return;
	}

	// Download pfs file
	int pfsDownloadResult = network.downloadFile(targetFile, 96);
	if (pfsDownloadResult == -1) {
		char msg[32];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "upload.download.error.%i", errno);
		network.sendResponse(msg);
		return;
	}

	// Download raw save
	memset(targetFile, 0, sizeof(targetFile));
	sprintf(targetFile, "/user/home/%x/savedata/%s/sdimg_%s", outUserId, clientSession->titleId, clientSession->dirName);
	int32_t fileSize = clientSession->saveBlocks * 32768;
	int rawDownloadResult = network.downloadFile(targetFile, fileSize);
	if (rawDownloadResult == -1) {
		char msg[32];
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "upload.download.error.%i", errno);
		network.sendResponse(msg);
		return;
	}
	network.sendResponse("ok");

}
