#pragma once

#include "commands/base.hpp"

class DumpSaveContainerCommand : public Command {
public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;

};
