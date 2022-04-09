#include <string.h>

#include "commands/reservesavecontainer.hpp"

#include "commands/events/phases.hpp"

#include "orbis/_types/save_data.h"

struct ReserveSaveContainerPacket {
	char titleId[16];
	char dirName[32];
	uint64_t saveBlocks;
};

struct ReserveSaveContainerResponse {
	char response[64];
};


bool isValidTitleId(const char * titleId) {
	for(int i = 0; i < 4; i++) {
		char letter = titleId[i];
		if(letter < 'A' || letter > 'Z') {
			return false;
		}
	}
	for(int i = 4; i < 10; i++) {
		char number = titleId[i];
		if (number < '0' || number > '9') {
			return false;
		} 
	}
	for(int i = 10; i < 16;i++) {
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
			Event e;
			e.name = "PhaseChangeEvent";
			CommandSystemStates state = CommandSystemStates::Done;
			e.value = (void*)&state;
			this->Notify(e);
			return;
		}

		ReserveSaveContainerResponse response;
		memset(&response, 0, sizeof(response));

		if (!isValidTitleId(packet.titleId)) {
			strcpy(response.response, "invalid.titleId");
			network.writeFull(&response);
			continue;
		}
		
		if (packet.saveBlocks < ORBIS_SAVE_DATA_BLOCKS_MIN2 || packet.saveBlocks > ORBIS_SAVE_DATA_BLOCKS_MAX) {
			strcpy(response.response, "invalid.saveBlocks");
			network.writeFull(&response);
			continue;
		} 
		
		int index = sessions.Reserve(packet.titleId, packet.dirName);
		if(index == -1) {
			strcpy(response.response, "reserve.tryagain");
			network.writeFull(&response);
			continue;
		}
		sessionIndex = index;

		ClientSession * clientSession = sessions.Get(index);
		memset(clientSession->mountPath, 0, ORBIS_SAVE_DATA_MOUNT_POINT_DATA_MAXSIZE);

		clientSession->saveBlocks = packet.saveBlocks;
		strcpy(response.response, "ok");
		network.writeFull(&response);
		break;
	}

	if (sessionIndex >= 0) {
		Event e;
		e.name = "PhaseChangeEvent";
		CommandSystemStates state = CommandSystemStates::MainLoop;
		e.value = (void*)&state;
		this->Notify(e);
		return;
	} else {
		Event e;
		e.name = "PhaseChangeEvent";
		CommandSystemStates state = CommandSystemStates::Done;
		e.value = (void*)&state;
		this->Notify(e);
		return;
	}
}

