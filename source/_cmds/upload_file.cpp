// Auto generated
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

#include "server_common.hpp"
#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"

int upload_file(int connfd, cmd_args & args) {
	char path[256];
	memset(&path, 0, sizeof(path));
	
	if (args.arg1 > 255) {
		sendResponse(connfd, "Must be less than 256 characters", CMD_RESPONSE_ERROR,0x1, 0);
		return CMD_FAILED;
	}

	if (args.arg2 == 0) {
		sendResponse(connfd, "File size must be greater than zero", CMD_RESPONSE_ERROR, 0x2, 0);
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
	

	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0) {
		log("Failed to open %s", path);
		sendResponse(connfd, "Failed to open file", CMD_RESPONSE_ERROR, 0, errno);
		return CMD_FAILED;
	}

	sendResponse(connfd, "", CMD_RESPONSE_READY, 0,0);

	size_t downloadStatus = downloadFile(connfd, fd, args.arg2);
	if (downloadStatus < 0) {
		log("Failed to download to %s: %i", path, downloadStatus);
		return CMD_FATAL;
	} else if (downloadStatus == 0) {
		log("Client disconnected before finishing their upload.");
		log("Removing %s...", path);
		close(fd);
		remove(path);
		return CMD_FAILED;
	}

	return CMD_SUCCESS;
}
