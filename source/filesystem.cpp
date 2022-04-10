#include "filesystem.hpp"

#include <string>
#include <vector>
#include <dirent.h>

void folderWalker(const char * root, const char * relativeFolder, std::function<void(const char *, const char *)> cb) {
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
			sprintf(sourcePath, "%s%s", relativeFolder, file->d_name);
			cb(root, sourcePath);
		}
	}
	closedir(srcDir);

	// TODO: Memory might be tight for a thread
	// If it becomes a problem then 
	// optimize for less recursion
	char newRelativeDirectory[128];

	for(std::string folderName: folders) {
		memset(newRelativeDirectory, 0, sizeof(newRelativeDirectory));
		sprintf(newRelativeDirectory, "%s%s/", relativeFolder, folderName.c_str());
		folderWalker(root, newRelativeDirectory, cb);
	}

}
