#pragma once

#include <stdint.h>

const uint32_t CMD_RESPONSE_READY = 0;
const uint32_t CMD_RESPONSE_INFO = 1;
const uint32_t CMD_RESPONSE_WARNING = 2;
const uint32_t CMD_RESPONSE_ERROR = 3;
const uint32_t CMD_RESPONSE_FATAL = 4;


const int CMD_UNKNOWN = 0;
const int CMD_SUCESS = 1;
const int CMD_FAILED = 2;
const int CMD_FATAL = 3;

#define CMD_ARGS_NAME_SIZE 0x20

const uint32_t SIZEOF_CMD_ARGS = CMD_ARGS_NAME_SIZE + sizeof(uint64_t); 

struct cmd_args {
	char name[0x20];
	uint64_t arg1;
	uint64_t arg2;
	uint64_t arg3;
	uint64_t arg4;
};

// TODO: Think of better data structure
struct cmd_response {
    char msg[0x40]; // Any human readable message
    uint32_t type; // CMD_RESPONSE_*
    uint32_t errorCode1; // Our error code
    uint32_t errorCode2; // Any other error code
};

typedef int (*cmd_function)(int connfd, cmd_args & args);

int32_t getUserId();

