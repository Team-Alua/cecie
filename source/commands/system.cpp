#include "log.hpp"

#include "commands/system.hpp"

CommandSystem::CommandSystem() {
	phaseEventSystem = CommandEventSystem(EventNamePhaseChange);
} 

void CommandSystem::Initialize() {
	this->AddCommandSet(CommandSystemStates::ReserveLoop, &reserveSet);
	this->ChangeToCommandSet(CommandSystemStates::ReserveLoop);
	this->AddCommandSet(CommandSystemStates::MainLoop, &mainSet);
	this->AddCommandSet(CommandSystemStates::ModifyLoop, &modifySet);

	phaseEventSystem.AddEventListener([](Event& e, void * instance) {
		auto cs = ((CommandSystem *) instance);
		auto state = *(CommandSystemStates*)e.value;
		if (state == CommandSystemStates::Done) {
			cs->MarkFinished();
			return;
		}
		cs->ChangeToCommandSet(state);
	}, this);
	this->AddEventPublishers(&phaseEventSystem);
}


void CommandSystem::AddEventPublishers(CommandEventSystem * eventSystem) {
	for(auto it = comSets.begin(); it != comSets.end(); ++it) {
		auto commandSet = it->second;
		commandSet->AddEventSystem(eventSystem);
	}
	log("Registered command event system for event %s", eventSystem->EventName());
}

void CommandSystem::AddCommandSet(CommandSystemStates state, CommandSet * cSet) {
	comSets[state] = cSet;
}

void CommandSystem::ChangeToCommandSet(CommandSystemStates state) {
	if (comSets.find(state) != comSets.end()) {
		currSet = comSets.at(state);
	}
}


CommandSet * CommandSystem::GetCommandSet() {
	return currSet;
}

bool CommandSystem::Finished() {
	return finished;
}

void CommandSystem::MarkFinished() {
	finished = true;
}

