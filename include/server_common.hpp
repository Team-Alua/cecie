#pragma once

#include <string.h>
#include <stddef.h>
#include <unistd.h>
#include <stdint.h>

int sendResponse(int connfd, const char * msg, uint32_t type, uint32_t err1, uint32_t err2);

template<class S>
int readFull(int connfd, S * serialized) {
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

int readFull(int connfd, void * buffer, size_t size);

int sendFile(int connfd, int fd, size_t size); 

int sendFull(int connfd, void * buffer, size_t size);

