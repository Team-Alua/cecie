#include "commands/set/main.hpp"

void MainSet::Initialize() {
	cmds["MountSaveContainer"] = &mountSaveContainerCommand;
	cmds["UnmountSaveContainer"] = &unmountSaveContainerCommand;
	cmds["DumpSaveContainer"] = &dumpSaveContainerCommand;
	cmds["ModifySaveContainer"] = &modifySaveContainerCommand;
	cmds["ResetSaveContainer"] = &resetSaveContainerCommand;
	cmds["ListFilesInSaveContainer"] = &listFilesInSaveContainerCommand;
	cmds["DeleteSaveContainer"] = &deleteSaveContainerCommand;
}

const char * MainSet::Name() {
	return "MainSet";
}

