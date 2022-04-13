#pragma once

#include "commands/set/base.hpp"


#include "commands/savecontainer/downloadfile.hpp"
#include "commands/savecontainer/uploadfile.hpp"
#include "commands/savecontainer/finish.hpp"

class ModifySet : public CommandSet {
private:
	DownloadFileFromSaveContainerCommand downloadFileFromSaveContainerCommand;
	UploadFileToSaveContainerCommand uploadFileToSaveContainerCommand;
	FinishModifyingSaveContainerCommand finishModifyingSaveContainerCommand;
public:
	virtual void Initialize() override;

	virtual const char * Name() override;
};
