#include "commands/system.hpp"

CommandSystem::CommandSystem() {
	phaseEventSystem = CommandEventSystem("PhaseChangeEvent");
} 

void CommandSystem::Initialize() {
	phaseEventSystem.AddEventListener([](Event& e, void * instance) {
		if (strcmp(e.name, "PhaseChangeEvent") == 0) {
			auto cs = ((CommandSystem *) instance);
			cs->ChangeToCommandSet((const char *)e.value);
		}
	}, this);

	this->AddEventPublishers(&phaseEventSystem);
}

void CommandSystem::AddEventPublishers(CommandEventSystem * eventSystem) {
	for(auto it = comSets.begin(); it != comSets.end(); ++it) {
		auto commandSet = it->second;
		commandSet->AddEventSystem(eventSystem);
	}
}

void CommandSystem::AddCommandSet(const char * name, CommandSet * cSet) {
	comSets[name] = cSet;
}

void CommandSystem::ChangeToCommandSet(const char * name) {
	if (comSets.find(name) != comSets.end()) {
		currSet = comSets.at(name);
	}
}


CommandSet * CommandSystem::GetCommandSet() {
	return currSet;
}

