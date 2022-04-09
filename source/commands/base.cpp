#include "commands/base.hpp"

void Command::AddEventSystem(CommandEventSystem & newEventSystem) {
	eventSystems.push_back(newEventSystem);
}

void Command::Notify(Event& e) {
	for(auto it = eventSystems.begin(); it != eventSystems.end(); ++it) {
		it->Notify(e);
	}
}

