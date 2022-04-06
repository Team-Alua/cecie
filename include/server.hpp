#pragma once
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <orbis/libkernel.h>


#define CMD_SERVER_PORT 8766

void cmdServer();

