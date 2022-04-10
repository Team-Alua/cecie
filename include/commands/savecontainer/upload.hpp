#pragma once

#include "commands/base.hpp"

class UploadSaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;
};

