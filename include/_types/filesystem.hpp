#pragma once

#include <stdint.h>

struct FileMetadata {
	char path[128];
	uint64_t size;
};

