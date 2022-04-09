#pragma once

#include "sessions.hpp"
#include "commands/events/basesystem.hpp"

class Command {
private:
	std::vector<CommandEventSystem> eventSystems;

public:
	virtual void Execute(int connfd, int sessionIndex, Sessions & sessions) = 0;

	void AddEventSystem(CommandEventSystem & newEventSystem);

	void Notify(Event& e);
};

