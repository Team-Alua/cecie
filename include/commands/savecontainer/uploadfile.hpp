#pragma once

#include "commands/base.hpp"

class UploadFileToSaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;
};

