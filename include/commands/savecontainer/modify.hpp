#pragma once

#include "commands/base.hpp"

class ModifySaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & sessionindex, Sessions & sessions) override;
};

