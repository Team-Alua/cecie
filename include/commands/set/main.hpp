#pragma once

#include "commands/set/base.hpp"

#include "commands/savecontainer/mount.hpp"
#include "commands/savecontainer/unmount.hpp"
#include "commands/savecontainer/dump.hpp"
#include "commands/savecontainer/modify.hpp"
#include "commands/savecontainer/reset.hpp"
#include "commands/savecontainer/listfiles.hpp"

class MainSet : public CommandSet {
private:
	MountSaveContainerCommand mountSaveContainerCommand;
	UnmountSaveContainerCommand unmountSaveContainerCommand;
	DumpSaveContainerCommand dumpSaveContainerCommand;
	ModifySaveContainerCommand modifySaveContainerCommand;
	ResetSaveContainerCommand resetSaveContainerCommand;
	ListFilesInSaveContainerCommand listFilesInSaveContainerCommand;
public:
	virtual void Initialize() override;

	virtual const char * Name() override;

};
