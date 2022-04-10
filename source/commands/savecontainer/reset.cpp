
#include <dirent.h>
#include <vector>

#include "commands/savecontainer/reset.hpp"

#include "constants.hpp"
#include "log.hpp"
#include "filesystem.hpp"

void ResetSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) == 0) {
		network.sendResponse("reset.notmounted");
		return;
	}
	network.sendResponse("ok");

	std::vector<std::string> filesToKeep = {
		"sce_sys/param.sfo",
		"sce_sys/keystone",
	};
	char mountFolder[64];
	memset(mountFolder, 0, sizeof(mountFolder));
	sprintf(mountFolder, "/mnt/sandbox/%s_000%s/", TITLE_ID, clientSession->mountPath);
	
	folderWalker(mountFolder, "", [&](const char * root, const char * relativePath) {
		char sourcePath[128];
		memset(sourcePath, 0, sizeof(sourcePath));
		sprintf(sourcePath, "%s%s", root, relativePath);
		if(std::find(filesToKeep.begin(), filesToKeep.end(), relativePath) == filesToKeep.end()) {
			log("Removing .. %s", sourcePath);
			remove(sourcePath);
		} else {
			log("Keeping .. %s", sourcePath);
		}
	});
}

