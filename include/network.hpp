#pragma once

#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

template<class S>
ssize_t readFull(int connfd, S * serialized) {
	size_t offset = 0;
	size_t size = sizeof(S);
	while (size > 0) {
		char * data = (char*)serialized + offset;
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

ssize_t readFull(int connfd, void * buffer, size_t size);


ssize_t sendFull(int connfd, void * buffer, size_t size);

ssize_t uploadFile(int connfd, int fd, size_t size);

ssize_t downloadFile(int connfd, int fd, size_t fileSize);

