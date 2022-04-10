#include "commands/savecontainer/listfiles.hpp"

#include "constants.hpp"
#include "filesystem.hpp"
#include "log.hpp"

void ListFilesInSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) == 0) {
		network.sendResponse("listfiles.notmounted");
		return;
	}
	network.sendResponse("ok");

	char mountFolder[64];
	memset(mountFolder, 0, sizeof(mountFolder));
	sprintf(mountFolder, "/mnt/sandbox/%s_000%s/", TITLE_ID, clientSession->mountPath);

	std::vector<std::string> files;
	folderWalker(mountFolder, "", [&](const char * root, const char * relativePath) {
		files.push_back(relativePath);
	});

	int32_t fileCount = files.size();
	network.writeFull(&fileCount);

	for(std::string file: files) {
		const char * fileName = file.c_str();

		int32_t fileNameLength = strlen(fileName);
		network.writeFull(&fileNameLength);

		log("Sending name: %s", fileName);
		int result = network.writeFull((void*)fileName, fileNameLength);

		// Client disconnected
		if (result == 0) {
			break;
		}
	}
}

