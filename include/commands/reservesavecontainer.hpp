#pragma once

#include "network.hpp"
#include "commands/base.hpp"
#include "sessions.hpp"
#include "commands/events/names.hpp"

class ReserveSaveContainerCommand: public Command {
public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) override;
};
