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

bool touch_save(int connfd, cmd_args & args) {

	struct cmd_response response;
	memset(&response, 0, sizeof(cmd_response));

	TouchSavePacket packet;
	memset(&packet, 0, sizeof(TouchSavePacket));

	int bytesRead = readFull<TouchSavePacket>(connfd, &packet);
	if (bytesRead <= 0) {
		return false;
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

		sprintf(response.msg, "Failed to mount %s for %s", packet.dirName, packet.titleId); 
		response.type = CMD_RESPONSE_ERROR;
		response.errorCode2 = mResult;

		log("There was an issue mount %s: 0x%04x", packet.dirName, mResult);
		return false;
	}

	OrbisSaveDataMountPoint mp;
	memset(&mp, 0, sizeof(OrbisSaveDataMountPoint));
	strcpy(mp.data, mountResult.mountPathName);

	int unResult = sceSaveDataUmount(&mp);
	if (mResult < 0) {
		sprintf(response.msg, "Failed to unmount %s for %s", packet.dirName, packet.titleId);
		response.type = CMD_RESPONSE_ERROR;
		response.errorCode2 = unResult;

		log("There was an issue unmounting %s: 0x%04x", packet.dirName, unResult);
		return false;
	}

	return true;
}
