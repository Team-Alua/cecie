
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>

#include "log.hpp"

#include <vector>

#include "commands/savecontainer/dump.hpp"

#include "constants.hpp"

struct FileDumpResponse {
	char path[128];
	uint64_t size;
};

/**
 * Root folder requires a /, relativeFolder does not
 */
void recursiveGetFiles(const char * root, const char * relativeFolder, std::vector<FileDumpResponse> & container) {
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

			FileDumpResponse dump;
			memset(&dump, 0, sizeof(dump));
			strcpy(dump.path, sourcePath);

			memset(sourcePath, 0, sizeof(sourcePath));
			sprintf(sourcePath, "%s%s/%s", root, relativeFolder, file->d_name);
			struct stat buf;
			memset(&buf, 0, sizeof(struct stat));
			int result = stat(sourcePath, &buf);
			if (result == -1) {
				log("There was an issue checking size of %s - %i", sourcePath, errno);
				continue;
			}
			dump.size = buf.st_size;

			container.push_back(dump);
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
		recursiveGetFiles(root, newRelativeDirectory, container);
	}

}

void DumpSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) == 0) {
		network.sendResponse("dump.error.notmounted");
		return;
	} else {
		network.sendResponse("ok");
	}

	char mountFolder[64];
	memset(mountFolder, 0, sizeof(mountFolder));
	sprintf(mountFolder, "/mnt/sandbox/%s_000%s/", TITLE_ID, clientSession->mountPath);


	std::vector<FileDumpResponse> container;
	recursiveGetFiles(mountFolder,"", container);


	int32_t fileCount = container.size();


	network.writeFull(&fileCount);

	for(FileDumpResponse fileDumpResponse: container) {
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
