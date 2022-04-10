#pragma once

#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

class Network {
private:
	int connfd;
public:
	Network(int fd): connfd(fd) {}
	
	template<class S>
	ssize_t readFull(S * serialized) {
		size_t offset = 0;
		size_t size = sizeof(S);
		while (size > 0) {
			char * data = (char*)serialized + offset;
			size_t readSize = 1024;
			if(size < readSize) {
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

	ssize_t readFull(void * buffer, size_t size);

	template<class S>
	ssize_t writeFull(S * serialized) {
		size_t offset = 0;
		size_t size = sizeof(S);
		while(size > 0) {
			char * data = (char*)serialized + offset;

			size_t writeSize = 1024;
			if(size < writeSize) {
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


	ssize_t sendResponse(const char * msg);

	ssize_t writeFull(void * buffer, size_t size);

	ssize_t uploadFile(const char * path, size_t size);

	ssize_t uploadFile(int fd, size_t size);

	ssize_t downloadFile(int fd, size_t size);
};

