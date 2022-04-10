#pragma once

#include "commands/base.hpp"

class DownloadSaveContainerCommand : public Command {
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;
};

