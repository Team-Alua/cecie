#pragma once

#include "commands/set/base.hpp"


#include "commands/savecontainer/downloadfile.hpp"
#include "commands/savecontainer/uploadfile.hpp"

class ModifySet : public CommandSet {
private:
	DownloadFileFromSaveContainerCommand downloadFileFromSaveContainerCommand;
	UploadFileToSaveContainerCommand uploadFileToSaveContainerCommand;
public:
	virtual void Initialize() override;

	virtual const char * Name() override;
};
