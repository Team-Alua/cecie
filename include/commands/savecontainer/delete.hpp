#pragma once

#include "commands/base.hpp"

class DeleteSaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;
};
