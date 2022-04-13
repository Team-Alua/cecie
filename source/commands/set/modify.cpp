#include "commands/set/modify.hpp"

void ModifySet::Initialize() {
	cmds["DownloadFile"] = &downloadFileFromSaveContainerCommand;
	cmds["UploadFile"] = &uploadFileToSaveContainerCommand;
	cmds["Finish"] = &finishModifyingSaveContainerCommand;
}

const char * ModifySet::Name() {
	return "ModifySet";
}

