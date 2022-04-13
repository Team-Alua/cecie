#pragma once

#include <functional>

struct FileMetadata {
	char path[128];
	uint64_t size;
};

void folderWalker(const char * root, const char * relativePath,  std::function<void(const char*, const char *)>);

