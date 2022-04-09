#pragma once

#include "commands/set/base.hpp"
#include "commands/savecontainer/reserve.hpp"

class ReserveSet : public CommandSet {
private:
	ReserveSaveContainerCommand reserveCommand;

public:
	virtual void Initialize() override;

	virtual const char * Name() override;

};
