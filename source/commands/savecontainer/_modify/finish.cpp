#include "commands/savecontainer/_modify/finish.hpp"

#include "commands/events/names.hpp"
#include "commands/events/phases.hpp"

void FinishModifyingSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	CommandSystemStates state = CommandSystemStates::MainLoop;
	this->Notify(EventNamePhaseChange, &state);
	network.sendResponse("ok");
}

