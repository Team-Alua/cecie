#include "commands/reserveset.hpp"

void ReserveSet::Initialize() {
	cmds["ReserveSaveContainer"] = &reserveCommand;
}

