#pragma once

#include "commands/set/base.hpp"


#include "commands/savecontainer/_modify/downloadfile.hpp"
#include "commands/savecontainer/_modify/uploadfile.hpp"
#include "commands/savecontainer/_modify/finish.hpp"

class ModifySet : public CommandSet {
private:
	DownloadFileFromSaveContainerCommand downloadFileFromSaveContainerCommand;
	UploadFileToSaveContainerCommand uploadFileToSaveContainerCommand;
	FinishModifyingSaveContainerCommand finishModifyingSaveContainerCommand;
public:
	virtual void Initialize() override;

	virtual const char * Name() override;
};
