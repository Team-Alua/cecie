#include "cmd_server.hpp"
#include "cmd_list.hpp"
#include "server_common.hpp"


static void cmdClientHandler(int connfd);

CommandHandler cmdHandler;

void cmdServer() {
	cmd_init(cmdHandler);

	log("Inside of cmdServer");
	int sockfd;
	int connfd;

	socklen_t addrLen;

	struct sockaddr_in serverAddr;
	struct sockaddr_in clientAddr;


	sockfd = socket(AF_INET,SOCK_STREAM, 0);

	if (sockfd < 0) {
		log("Failed to create port: %lx", errno);
		// Send error here
		return;
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(CMD_SERVER_PORT); // TODO: Do not hardcode this
	
	log("About to bind.");
	// Repeat until it does bind
	while (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
	}
	log("Binded to port.");
	if (listen(sockfd, 5) != 0) {
		log("Failed to listen: %lx", errno);
		// Provide error here
		return;
	}


	addrLen = sizeof(clientAddr);

	for(;;) {
		int connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
		if (connfd < 0) {
			log("Failed to accept client: %04x", errno);
			break;
		}
		std::thread clientThread(cmdClientHandler, connfd);
		clientThread.detach();
	}
	close(sockfd);
}

static void cmdClientHandler(int connfd) {
	// Add all available commands
	while(true) {
		// This should check if a command exists or not
		log("Expecting packet size of %u from client.", sizeof(cmd_args));

		struct cmd_args args;
		memset(&args, 0, sizeof(args));

		int readBytes = readFull<cmd_args>(connfd, &args);
		if (readBytes == 0) {
			break;
		} else if (readBytes < 0) {
			break;
		} else if (readBytes < sizeof(cmd_args)) {
			break;
		} else if (readBytes > sizeof(cmd_args)) {
			log("Received more %u instead of %u", readBytes, sizeof(cmd_args));
			break;
		}

		log("Received name:%s args: %lx %lx %lx %lx", args.name, args.arg1, args.arg2, args.arg3, args.arg4);

		int cmdResult = cmdHandler.call(args.name, connfd, args);
		struct cmd_response response;
		memset(&response, 0, sizeof(response));
		if (cmdResult == CMD_UNKNOWN) {
			// Command doesn't exist
			sprintf(response.msg, "%s is not a command", args.name);
			response.type = CMD_RESPONSE_ERROR;
			response.errorCode1 = 0x0;
			// While it's unlikely that this will not send 
			// in one go, should port writeFull here
		} else if (cmdResult == CMD_FAILED) {
			// Command sent their own response so do not send one
			continue;
		} else if (cmdResult == CMD_FATAL) {
			// Something bad happen
			break;
		} else {
			// Send default response
			response.type = CMD_RESPONSE_READY;
		}
		write(connfd, &response, sizeof(response));
	}
	close(connfd);
}
