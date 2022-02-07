// Auto generated
#include <stdio.h>
#include <errno.h>

#include "server_common.hpp"
#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"

int delete_file(int connfd, cmd_args & args) {
	char path[256];
	memset(&path, 0, sizeof(path));
	
	if (args.arg1 > 255) {
		sendResponse(connfd, "Must be less than 256 characters", CMD_RESPONSE_ERROR,0x1, 0);
		return CMD_FAILED;
	}

	sendResponse(connfd, "", CMD_RESPONSE_READY, 0,0);

	int bytesRead = readFull(connfd, &path, args.arg1);
	if (bytesRead < 0) {
		log("An error occured while trying to read from socket: %i", errno);
		return CMD_FAILED;
	} else if (bytesRead == 0) {
		// Client disconnected
		return CMD_FAILED;
	} else if (bytesRead < args.arg1) {
		log("Read less than expected amount.");
		return CMD_FAILED;
	}

	int result = remove(path);
	if (result < 0) {
		log("Failed to remove %s", path);
		sendResponse(connfd, "Failed to remove file.", CMD_RESPONSE_ERROR, 0, errno);
		return CMD_FAILED;
	}
	return CMD_SUCCESS;
}
