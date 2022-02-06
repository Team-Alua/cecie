// Auto generated
#include <sys/time.h>
#include <string.h>
#include <stdio.h>

#include <orbis/SaveData.h>

#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"
#include "server_common.hpp"

struct __attribute__ ((packed)) TouchSavePacket {
	char dirName[0x20];
	char titleId[0x10];
	char fingerprint[0x50];
	uint64_t saveBlocks;
};

int touch_save(int connfd, cmd_args & args) {
	TouchSavePacket packet;
	memset(&packet, 0, sizeof(TouchSavePacket));

	int bytesRead = readFull<TouchSavePacket>(connfd, &packet);
	if (bytesRead <= 0) {
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

		log("There was an issue mount %s: 0x%04x", packet.dirName, mResult);
		return CMD_FAILED;
	}

	OrbisSaveDataMountPoint mp;
	memset(&mp, 0, sizeof(OrbisSaveDataMountPoint));
	strcpy(mp.data, mountResult.mountPathName);

	int unResult = sceSaveDataUmount(&mp);
	if (mResult < 0) {
		char msg[64];
		memset(&msg, 0, sizeof(msg));
		sprintf(msg, "Failed to unmount %s for %s", packet.dirName, packet.titleId);

		sendResponse(connfd, msg, CMD_RESPONSE_ERROR, 0, unResult);

		log("There was an issue unmounting %s: 0x%04x", packet.dirName, unResult);
		return CMD_FAILED;
	}

	return CMD_SUCESS;
}
