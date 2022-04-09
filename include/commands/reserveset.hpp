#pragma once

#include "commands/set.hpp"
#include "commands/reservesavecontainer.hpp"

class ReserveSet : public CommandSet {
private:
	ReserveSaveContainerCommand reserveCommand;

public:
	virtual void Initialize() override;
};
