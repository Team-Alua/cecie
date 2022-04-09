#pragma once

#include <unordered_map>
#include <string>

#include "commands/set.hpp"
#include "commands/events/basesystem.hpp"

#include "commands/events/phases.hpp"

#include "commands/reserveset.hpp"
#include "commands/mainset.hpp"
#include "commands/modifyset.hpp"

class CommandSystem {
private:
	std::unordered_map<CommandSystemStates, CommandSet*> comSets;
	CommandSet * currSet = NULL;
	CommandEventSystem phaseEventSystem;
	
	ReserveSet reserveSet;
	MainSet mainSet;
	ModifySet modifySet;

	void AddEventPublishers(CommandEventSystem * eventSystem);
	
	bool finished = false;

public:
	CommandSystem();

	void Initialize();

	void AddCommandSet(CommandSystemStates name, CommandSet * cSet);

	void ChangeToCommandSet(CommandSystemStates state);

	CommandSet * GetCommandSet();

	void MarkFinished();

	bool Finished();
};

