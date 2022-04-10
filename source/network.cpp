#include "network.hpp" 

#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>

ssize_t Network::readFull(void * buffer, size_t size) {
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

ssize_t Network::uploadFile(int fd, size_t size) {
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

ssize_t Network::uploadFile(const char * path, size_t size) {
	int fd = open(path, O_RDONLY);
	if (fd == -1) {
		return -1;
	}

	ssize_t result = Network::uploadFile(fd, size);

	int err = errno;

	close(fd);

	errno = err;

	return result;
}

ssize_t Network::writeFull(void * buffer, size_t size) {
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

ssize_t Network::downloadFile(int fd, size_t fileSize) {

	uint8_t buffer[8192];

	uint32_t bytesRemaining = fileSize;

	do {
		// No more to download
		if (bytesRemaining == 0) {
			break;
		}

		size_t fileBufSize = 8192;
		if (bytesRemaining < fileBufSize) {
			fileBufSize = bytesRemaining;
		}

		ssize_t received = this->readFull(buffer, fileBufSize);
		// Error reading
		if (received <= 0) {
			return received;
		}
		while (received > 0) {
			size_t fileOffset = fileSize - bytesRemaining;
			ssize_t bytesWritten = pwrite(fd, buffer, received, fileOffset);
			if (bytesWritten < 0) {
				return bytesWritten;
			}
			if (bytesWritten == 0) {
				break;
			}
			bytesRemaining -= bytesWritten;
			received -= bytesWritten;
		}
	} while(true);

	// Return amount downloaded
	return fileSize - bytesRemaining;
}

ssize_t Network::downloadFile(const char * path, size_t size) {

	int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1) {
		return -1;
	}

	ssize_t result = Network::downloadFile(fd, size);
	int err = errno;

	close(fd);

	if (result == -1) {
		remove(path);
	}

	errno = err;
	return result;
}

ssize_t Network::sendResponse(const char * msg) {
	int msgLength = strlen(msg);
	
	ssize_t result = write(connfd, &msgLength, sizeof(msgLength));
	if (result <= 0) {
		return result;
	}
	return write(connfd, msg, msgLength);
}

