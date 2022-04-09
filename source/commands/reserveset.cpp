#include "commands/reserveset.hpp"

void ReserveSet::Initialize() {
	cmds["ReserveSaveContainer"] = &reserveCommand;
}

const char * ReserveSet::Name() {
	return "ReserveSet";
}

