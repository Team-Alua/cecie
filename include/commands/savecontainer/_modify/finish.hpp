#pragma once

#include "commands/base.hpp"

class FinishModifyingSaveContainerCommand : public Command {
public:
	virtual void Execute(Network&, int&, Sessions&) override;
};

