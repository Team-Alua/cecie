#include "commands/savecontainer/modify.hpp"

#include "commands/events/phases.hpp"
#include "commands/events/names.hpp"

void ModifySaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);
	if (strlen(clientSession->mountPath) == 0) {
		network.sendResponse("modify.notmounted");
		return;
	}
	CommandSystemStates phase = CommandSystemStates::ModifyLoop;
	this->Notify(EventNamePhaseChange, &phase);
	network.sendResponse("ok");
}

