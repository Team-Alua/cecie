#pragma once

#include <vector>
#include <functional>

#include "commands/events/event.hpp"


class CommandEventSystem {
protected:
	std::vector<std::function<void(Event&)>> eventListeners;

	void OnNotify(Event& event);

	const char * eventName;
public:

	CommandEventSystem(): eventName("") {}

	CommandEventSystem(const char * evtName): eventName(evtName) {}

	void AddEventListener(std::function<void(Event&)> cb);

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

