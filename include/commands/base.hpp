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

	template<class S>
	void Notify(const char * evtName, S * serialized) {
		for(auto it = eventSystems.begin(); it != eventSystems.end(); ++it) {
			(*it).Notify(evtName, serialized);
		}
	}
};

