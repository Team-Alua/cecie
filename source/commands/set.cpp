#include "commands/set.hpp"

void CommandSet::AddEventSystem(CommandEventSystem * eventSystem) {
	for(auto it = cmds.begin(); it != cmds.end(); it++) {
		auto cmdPair = *it;
		auto cmd = cmdPair.second.get();
		cmd->AddEventSystem(*eventSystem);
	}
}
