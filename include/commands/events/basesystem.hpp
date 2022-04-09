#pragma once

#include <vector>

#include "commands/events/event.hpp"
#include "commands/events/callback.hpp"


class CommandEventSystem {
protected:
	std::vector<EventCallbackPair> eventListeners;

	void OnNotify(Event& event);

	const char * name;
public:
	CommandEventSystem(): name("") {}

	CommandEventSystem(const char * eventName): name(eventName) {}

	void AddEventListener(EventCallback cb, void * instance);

	void Notify(Event& event);
};

