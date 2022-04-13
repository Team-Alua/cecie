#include "commands/events/basesystem.hpp"

#include "log.hpp"

void CommandEventSystem::AddEventListener(std::function<void(Event&)> cb) {
	eventListeners.push_back(cb);
}

void CommandEventSystem::OnNotify(Event& event) {
	std::vector<std::function<void(Event&)>>::iterator it = eventListeners.begin();
	for(;it != eventListeners.end(); ++it) {
		(*it)(event);
	}
}

void CommandEventSystem::Notify(Event& event) {
	if (strcmp(event.name, eventName) == 0) {
		log("Event %s was called.", event.name);
		this->OnNotify(event);
	}
}


const char * CommandEventSystem::EventName() {
	return eventName;
}

