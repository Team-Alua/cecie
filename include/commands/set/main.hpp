#pragma once

#include "commands/set/base.hpp"

#include "commands/savecontainer/mount.hpp"
#include "commands/savecontainer/unmount.hpp"
#include "commands/savecontainer/dump.hpp"

class MainSet : public CommandSet {
private:
	MountSaveContainerCommand mountSaveContainerCommand;
	UnmountSaveContainerCommand unmountSaveContainerCommand;
	DumpSaveContainerCommand dumpSaveContainerCommand;
public:
	virtual void Initialize() override;

	virtual const char * Name() override;

};
