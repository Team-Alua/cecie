#pragma once

#include "network.hpp"

#include "sessions.hpp"
#include "commands/events/basesystem.hpp"

class Command {
private:
	std::vector<CommandEventSystem> eventSystems;

public:
	virtual void Execute(Network & network, int & sessionIndex, Sessions & sessions) = 0;

	void AddEventSystem(CommandEventSystem & newEventSystem);

	void Notify(Event& e);
};

