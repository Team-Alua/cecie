#include "commands/set/main.hpp"

void MainSet::Initialize() {
	cmds["MountSaveContainer"] = &mountSaveContainerCommand;
	cmds["UnmountSaveContainer"] = &unmountSaveContainerCommand;
	cmds["DumpSaveContainer"] = &dumpSaveContainerCommand;
	cmds["ModifySaveContainer"] = &modifySaveContainerCommand;
	cmds["ResetSaveContainer"] = &resetSaveContainerCommand;
	cmds["ListFilesInSaveContainer"] = &listFilesInSaveContainerCommand;
	cmds["DeleteSaveContainer"] = &deleteSaveContainerCommand;
	cmds["DownloadSaveContainer"] = &downloadSaveContainerCommand;
	cmds["UploadSaveContainer"] = &uploadSaveContainerCommand;
}

const char * MainSet::Name() {
	return "MainSet";
}

