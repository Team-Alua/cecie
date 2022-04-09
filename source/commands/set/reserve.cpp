#include "commands/set/reserve.hpp"

void ReserveSet::Initialize() {
	cmds["ReserveSaveContainer"] = &reserveCommand;
}

const char * ReserveSet::Name() {
	return "ReserveSet";
}

