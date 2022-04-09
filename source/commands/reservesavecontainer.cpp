#include <string.h>

#include "commands/reservesavecontainer.hpp"

#include "commands/events/phases.hpp"

#include "orbis/_types/save_data.h"

#include "log.hpp"

struct ReserveSaveContainerPacket {
	char titleId[16];
	char dirName[32];
	uint64_t saveBlocks;
};

bool isValidTitleId(const char * titleId) {
	for(int i = 0; i < 4; i++) {
		char letter = titleId[i];
		if(letter < 'A' || letter > 'Z') {
			return false;
		}
	}
	for(int i = 4; i < 9; i++) {
		char number = titleId[i];
		if (number < '0' || number > '9') {
			return false;
		} 
	}
	for(int i = 9; i < 16;i++) {
		if (titleId[i] != 0) {
			return false;
		}
	}
	return true;
}

void ReserveSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ReserveSaveContainerPacket packet;
	while(sessionIndex < 0) {
		ssize_t result = network.readFull(&packet);
		if (result <= 0) {
			CommandSystemStates state = CommandSystemStates::Done;
			this->Notify(EventNamePhaseChange, &state);
			return;
		}


		if (!isValidTitleId(packet.titleId)) {
			network.sendResponse("invalid.titleId");
			continue;
		}
		
		if (packet.saveBlocks < ORBIS_SAVE_DATA_BLOCKS_MIN2 || packet.saveBlocks > ORBIS_SAVE_DATA_BLOCKS_MAX) {
			network.sendResponse("invalid.saveBlocks");
			continue;
		} 
		
		int index = sessions.Reserve(packet.titleId, packet.dirName);
		if(index == -1) {
			network.sendResponse("reserve.tryagain");
			continue;
		}
		sessionIndex = index;

		ClientSession * clientSession = sessions.Get(index);
		memset(clientSession->mountPath, 0, ORBIS_SAVE_DATA_MOUNT_POINT_DATA_MAXSIZE);

		clientSession->saveBlocks = packet.saveBlocks;
		network.sendResponse("ok");
		break;
	}

	if (sessionIndex >= 0) {
		CommandSystemStates state = CommandSystemStates::MainLoop;
		this->Notify(EventNamePhaseChange, &state);
	} else {
		CommandSystemStates state = CommandSystemStates::Done;
		this->Notify(EventNamePhaseChange, &state);
	}
}

