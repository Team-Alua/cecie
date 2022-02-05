#pragma once
#include <thread>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <orbis/libkernel.h>

#include "common.hpp"

#define PONG_PORT 8765

void pongServer();

