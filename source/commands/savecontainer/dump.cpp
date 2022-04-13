
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include <vector>


#include "log.hpp"
#include "constants.hpp"
#include "filesystem.hpp"

#include "commands/savecontainer/dump.hpp"


void DumpSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) == 0) {
		network.sendResponse("dump.error.notmounted");
		return;
	}
	network.sendResponse("ok");

	char mountFolder[64];
	memset(mountFolder, 0, sizeof(mountFolder));
	sprintf(mountFolder, "/mnt/sandbox/%s_000%s/", TITLE_ID, clientSession->mountPath);


	std::vector<FileMetadata> container;

	folderWalker(mountFolder, "", [&](const char * root, const char * relativePath){
		FileMetadata dump;
		memset(&dump, 0, sizeof(FileMetadata));
		sprintf(dump.path, "%s%s", root, relativePath);
		
		struct stat buf;
		memset(&buf, 0, sizeof(struct stat));
		int result = stat(dump.path, &buf);
		if (result == -1) {
			log("There was an issue checking size of %s - %i", dump.path, errno);
		}
		memset(&dump, 0, sizeof(FileMetadata));
		sprintf(dump.path, "%s", relativePath);
		dump.size = buf.st_size;
		container.push_back(dump);
	});

	int32_t fileCount = container.size();
	network.writeFull(&fileCount);

	for(FileMetadata fileDumpResponse: container) {
		log("Sending name: %s size: %u", fileDumpResponse.path, fileDumpResponse.size);
		network.writeFull(&fileDumpResponse);
		char targetFolder[256];
		memset(targetFolder, 0, sizeof(targetFolder));
		sprintf(targetFolder, "%s%s", mountFolder, fileDumpResponse.path);
		int result = network.uploadFile(targetFolder, fileDumpResponse.size);

		// Client disconnected
		if (result == 0) {
			break;
		}
	}

}

