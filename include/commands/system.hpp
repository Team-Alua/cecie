#pragma once

#include <unordered_map>
#include <string>

#include "commands/set.hpp"
#include "commands/events/basesystem.hpp"

class CommandSystem {
private:
	std::unordered_map<std::string, CommandSet*> comSets;
	CommandSet * currSet = NULL;
	CommandEventSystem phaseEventSystem;

	void AddEventPublishers(CommandEventSystem * eventSystem);

public:
	CommandSystem();
	void Initialize();

	void AddCommandSet(const char * name, CommandSet * cSet);

	void ChangeToCommandSet(const char * name);

	CommandSet * GetCommandSet();

};

