#include "commands/set/main.hpp"

void MainSet::Initialize() {
	cmds["MountSaveContainer"] = &mountSaveContainerCommand;
	cmds["UnmountSaveContainer"] = &unmountSaveContainerCommand;
	cmds["DumpSaveContainer"] = &dumpSaveContainerCommand;
}

const char * MainSet::Name() {
	return "MainSet";
}

