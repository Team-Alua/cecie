#pragma once

#include "commands/base.hpp"

class MountSaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & connfd, Sessions & sessions) override;
};

