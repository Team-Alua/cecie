#include "commands/set.hpp"

void CommandSet::AddEventSystem(CommandEventSystem * eventSystem) {
	for(auto it = cmds.begin(); it != cmds.end(); it++) {
		auto cmd = (*it).second;
		cmd->AddEventSystem(*eventSystem);
	}
}

Command* CommandSet::Get(const char * cmdName) {
	if (cmds.find(cmdName) != cmds.end()) {
		return cmds.at(cmdName);
	}

	return NULL;
}

