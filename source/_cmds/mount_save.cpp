// Auto generated
#include <string.h>
#include <stdio.h>
#include <sys/time.h>
#include <errno.h>

#include <orbis/SaveData.h>

#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"
#include "server_common.hpp"

struct __attribute__ ((packed)) MountPacket {
	char dirName[0x20];
	char titleId[0x10];
	char fingerprint[0x50];
	uint64_t saveBlocks;
};


int mount_save(int connfd, cmd_args & args) {
	MountPacket packet;
	memset(&packet, 0, sizeof(MountPacket));

	int bytesRead = readFull<MountPacket>(connfd, &packet);
	if (bytesRead == 0) {
		log("Client disconnected before they sent MountPacket.");
		return CMD_FAILED;
	} else if (bytesRead < 0) {
		log("Something went wrong... %i", errno);
		return CMD_FAILED;
	}

	log("dirName: %s titleId: %s fingerprint: %s saveBlocks: %lu",packet.dirName,
								      packet.titleId,
								      packet.fingerprint,
								      packet.saveBlocks);
	OrbisSaveDataMount mount;
	memset(&mount, 0, sizeof(OrbisSaveDataMount));

	mount.userId = getUserId();
	mount.dirName = packet.dirName;
	mount.titleId = packet.titleId;
	mount.fingerprint = packet.fingerprint;
	mount.blocks = packet.saveBlocks;

	mount.mountMode = ORBIS_SAVE_DATA_MOUNT_MODE_DESTRUCT_OFF;
	mount.mountMode |= ORBIS_SAVE_DATA_MOUNT_MODE_CREATE2;
	mount.mountMode |= ORBIS_SAVE_DATA_MOUNT_MODE_RDWR;

	OrbisSaveDataMountResult mountResult;
	memset(&mountResult, 0, sizeof(OrbisSaveDataMountResult));
	
	int mResult = sceSaveDataMount(&mount, &mountResult);
	if (mResult < 0) {
		char msg[64]; 
		memset(&msg, 0, sizeof(msg));

		sprintf(msg, "Failed to mount %s for %s", packet.dirName, packet.titleId); 
		sendResponse(connfd, msg, CMD_RESPONSE_ERROR, 0, mResult);

		log("There was an issue mounting %s: 0x%04x", packet.dirName, mResult);
		return CMD_FAILED;
	}
	sendResponse(connfd, mountResult.mountPathName, CMD_RESPONSE_READY, 0, 0);
	// So it doesn't send a response again
	return CMD_FAILED;
}
