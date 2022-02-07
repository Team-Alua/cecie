// Auto generated

#include <errno.h>
#include <fcntl.h>

#include "cmd_common.hpp"
#include "cmd_list.hpp"
#include "common.hpp"
#include "server_common.hpp"

int download_file(int connfd, cmd_args & args) {
	char path[256];
	memset(&path, 0, sizeof(path));
	
	if (args.arg1 > 255) {
		sendResponse(connfd, "Must be less than 256 characters", CMD_RESPONSE_ERROR,0x1, 0);
		return CMD_FAILED;
	}

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
	

	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		log("Failed to open %s", path);
		sendResponse(connfd, "Failed to open file", CMD_RESPONSE_ERROR, 0, errno);
		return CMD_FAILED;
	}

	// Get file size
	off_t fileSize = lseek(fd, 0, SEEK_END);
	if (fileSize == -1) {
		sendResponse(connfd, "Failed to seek to end of file", CMD_RESPONSE_ERROR, 0, errno);
		return CMD_FAILED;
	}

	if (lseek(fd, 0, SEEK_SET) == -1) {
		sendResponse(connfd, "Failed to seek to beginning of file", CMD_RESPONSE_ERROR, 0, errno);
		return CMD_FAILED;
	}

	// Notify the client we are ready to send information
	sendResponse(connfd, "", CMD_RESPONSE_READY, 0, 0);

	// Send file size
	write(connfd, &fileSize, sizeof(off_t));

	// Then send file content
	log("Now sending %s", path);
	int bytesSent = sendFile(connfd, fd, fileSize);
	if (bytesSent < 0) {
		// Some issue with sending the file
		log("There was an issue sending the file. %i", errno);
		return CMD_FATAL;
	}

	return CMD_SUCCESS;
}

