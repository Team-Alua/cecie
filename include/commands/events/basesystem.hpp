#pragma once

#include <vector>

#include "commands/events/event.hpp"
#include "commands/events/callback.hpp"


class CommandEventSystem {
protected:
	std::vector<EventCallbackPair> eventListeners;

	void OnNotify(Event& event);

	const char * eventName;
public:

	CommandEventSystem(): eventName("") {}

	CommandEventSystem(const char * evtName): eventName(evtName) {}

	void AddEventListener(EventCallback cb, void * instance);

	void Notify(Event& event);


	template<class S>
	void Notify(const char * evtName, S * serialized) {
		Event e;
		e.name = evtName;
		e.value = serialized;
		this->Notify(e);
	}

	const char * EventName();
};

