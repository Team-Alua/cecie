// Auto generated
#include "server_common.hpp"
#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"
#include <sys/time.h>
#include <orbis/SaveData.h>

struct __attribute__ ((packed)) DeleteSavePacket {
	char dirName[0x20];
	char titleId[0x10];
};

int delete_save(int connfd, cmd_args & args) {
	struct DeleteSavePacket delPack;
	int32_t bytesRead = readFull<DeleteSavePacket>(connfd, &delPack);
	if (bytesRead < 0) {
		// Something went wrong
		// Need to handle this case
		return CMD_FAILED;
	} else if (bytesRead == 0) {
		// Client disconnected;
		return CMD_FAILED;
	}
	log("titleId: %s dirName: %s", delPack.titleId, delPack.dirName);

	OrbisSaveDataDelete del;
	memset(&del, 0, sizeof(OrbisSaveDataDelete));
	del.userId = getUserId();
	del.dirName =  (OrbisSaveDataDirName*) &delPack.dirName;
	del.titleId = (OrbisSaveDataTitleId*) &delPack.titleId;
	

	int32_t result = sceSaveDataDelete(&del);
	log("Delete result %04x", result);
	// Note: During testing this never evaluated
	// to true even if the dirName is from a game that
	// doesn't exist
	if (result < 0) {
		sendResponse(connfd, "Failed to delete save.", CMD_RESPONSE_ERROR, 0, result);
		return CMD_FAILED;
	}

	return CMD_SUCCESS;
}
