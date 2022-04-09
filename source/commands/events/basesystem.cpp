#include "commands/events/basesystem.hpp"

void CommandEventSystem::AddEventListener(EventCallback cb, void * instance) {
	EventCallbackPair pair;
	pair.cb = cb;
	pair.instance = instance;
	eventListeners.push_back(pair);
}

void CommandEventSystem::OnNotify(Event& event) {
	std::vector<EventCallbackPair>::iterator it = eventListeners.begin();
	for(;it != eventListeners.end(); ++it) {
		auto pair = (*it);
		pair.cb(event, pair.instance);
	}
}

void CommandEventSystem::Notify(Event& event) {
	if (strcmp(event.name, eventName) == 0) {
		this->OnNotify(event);
	}
}


const char * CommandEventSystem::EventName() {
	return eventName;
}

