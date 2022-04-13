#include "commands/set/modify.hpp"

void ModifySet::Initialize() {
	cmds["DownloadFile"] = &downloadFileFromSaveContainerCommand;
	cmds["UploadFile"] = &uploadFileToSaveContainerCommand;
}

const char * ModifySet::Name() {
	return "ModifySet";
}

