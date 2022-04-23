#pragma once

#include "commands/base.hpp"

class DownloadFileFromSaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;
};
