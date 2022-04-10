
#include <dirent.h>
#include <vector>

#include "commands/savecontainer/reset.hpp"
#include "constants.hpp"

#include "log.hpp"

static void recursiveDeleteFiles(const char * root, const char * relativeFolder, std::vector<std::string> & filesToKeep);

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

	recursiveDeleteFiles(mountFolder, "", filesToKeep);
}

/**
 * Root folder requires a /, relativeFolder does not
 */
static void recursiveDeleteFiles(const char * root, const char * relativeFolder, std::vector<std::string> & filesToKeep) {
	char targetFolder[256];
	memset(targetFolder, 0, sizeof(targetFolder));
	strcpy(targetFolder, root);
	strcat(targetFolder, relativeFolder);


	DIR * srcDir = opendir(targetFolder);
	struct dirent * file;
	std::vector<std::string> folders;

	while ((file = readdir(srcDir)) != NULL) {
		if(strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
			continue;
		}
		if (file->d_type == DT_DIR) {
			folders.push_back(std::string(file->d_name));
			continue;
		}
		if (file->d_type == DT_REG) {
			char sourcePath[128];
			memset(sourcePath, 0, sizeof(sourcePath));
			sprintf(sourcePath, "%s/%s", relativeFolder, file->d_name);

			if (std::find(filesToKeep.begin(), filesToKeep.end(), sourcePath) == filesToKeep.end()) {
				memset(sourcePath, 0, sizeof(sourcePath));
				sprintf(sourcePath, "%s%s/%s", root, relativeFolder, file->d_name);
				log("Removing .. %s", sourcePath);
				remove(sourcePath);
			} else {
				memset(sourcePath, 0, sizeof(sourcePath));
				sprintf(sourcePath, "%s%s/%s", root, relativeFolder, file->d_name);
				log("Keeping .. %s", sourcePath);
			}
		}
	}
	closedir(srcDir);

	// TODO: Memory might be tight for a thread
	// If it becomes a problem then 
	// optimize for less recursion
	char newRelativeDirectory[128];

	for(std::string folderName: folders) {
		memset(newRelativeDirectory, 0, sizeof(newRelativeDirectory));
		sprintf(newRelativeDirectory, "%s%s", relativeFolder, folderName.c_str());
		recursiveDeleteFiles(root, newRelativeDirectory, filesToKeep);
	}
}

