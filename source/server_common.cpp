#include "cmd_common.hpp"
#include "server_common.hpp" 

int readFull(int connfd, void * buffer, size_t size) {
	size_t offset = 0;
	while (size > 0) {
		char * data = (char*)buffer + offset;
		size_t readSize = 1024;
		if (size < readSize) {
			readSize = size;
		}
		ssize_t bytesRead = read(connfd, data, readSize);
		if (bytesRead <= 0) {
			return bytesRead;
		} 
		offset += bytesRead;
		size -= bytesRead;
	}
	return offset;
}

int sendFile(int connfd, int fd, size_t size) {
	unsigned char buffer[4096];
	while (size > 0) {
		size_t bufferRead = size < 4096ul ? size: 4096ul;

		ssize_t bytesRead = read(fd, buffer, bufferRead);
		// Some issue or zero bytes left for some reason
		if (bytesRead <= 0) {
			return bytesRead;
		}
		
		size_t totalBytesWritten = 0;
		// Send all read bytes to the other side
		while (totalBytesWritten < bytesRead) {
			ssize_t bytesWritten = write(connfd,&buffer[totalBytesWritten], bytesRead - totalBytesWritten);
			//  Some issue with writing
			if (bytesWritten <= 0) {
				return bytesWritten;
			}
			totalBytesWritten += bytesWritten;
		}
		// Update size to account for written bytes
		size -= totalBytesWritten;
	}

	// Success
	return 1;
}

int sendFull(int connfd, void * buffer, size_t size) {
	size_t offset = 0;
	while (size > 0) {
		char * data = (char*)buffer + offset;
		size_t writeSize = 1024;
		if (size < writeSize) {
			writeSize = size;
		}
		ssize_t bytesWritten = write(connfd, data, writeSize);
		if (bytesWritten <= 0) {
			return bytesWritten;
		} 

		offset += bytesWritten;
		size -= bytesWritten;
	}
	return offset;
}

int sendResponse(int connfd, const char * msg, uint32_t type, uint32_t err1, uint32_t err2) {
	struct cmd_response response;
	memset(&response, 0, sizeof(cmd_response));
	strcpy(response.msg, msg);
	response.type = type;
	response.errorCode1 = err1;
	response.errorCode2 = err2;
	
	return sendFull(connfd, &response, sizeof(cmd_response));
}

