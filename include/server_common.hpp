#pragma once


#include <stddef.h>
#include <unistd.h>

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

