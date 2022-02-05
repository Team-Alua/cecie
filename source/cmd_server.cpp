#include "cmd_server.hpp"

static void cmdClientHandler(int connfd);

void cmdServer() {
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
	while(true) {
		close(connfd);
		break;
	}
}
