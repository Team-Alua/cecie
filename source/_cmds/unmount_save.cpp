// Auto generated


#include <sys/time.h>
#include <errno.h>

#include <orbis/SaveData.h>

#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"
#include "server_common.hpp"

int unmount_save(int connfd, cmd_args & args) {
	log("UnmountSave was called!"); 
	OrbisSaveDataMountPoint mp;
	int bytesRead = readFull<OrbisSaveDataMountPoint>(connfd, &mp);
	
	if (bytesRead == 0) {
		// Client disconnected
		return CMD_FAILED;
	} else if (bytesRead < 0) {
		log("Error occured: %i", errno);
		return CMD_FAILED;
	}
	log("Received mountPath:%s", mp.data);

	int32_t unResult = sceSaveDataUmount(&mp);
	if (unResult < 0) {
		log("Failed to unmount %s: 0x%04x", mp.data, unResult);
		sendResponse(connfd, "Failed to unmount %s", CMD_RESPONSE_ERROR, 0, unResult);
		return CMD_FAILED;
	}

	return CMD_SUCCESS;
}
