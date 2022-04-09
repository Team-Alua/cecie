#include "log.hpp"
#include "server.hpp"
#include "sessions.hpp"
#include "network.hpp"
#include "clienthandler.hpp"

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
	serverAddr.sin_port = htons(CMD_SERVER_PORT);
	
	log("About to bind.");
	system_notification("cmd server starting binding attempts.");
	// Repeat until it does bind
	while (bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) != 0) {
	}
	log("Binded to port.");
	if (listen(sockfd, 5) != 0) {
		system_notification("cmd server failed to listen.");
		log("Failed to listen: %lx", errno);
		// Provide error here
		return;
	}

	system_notification("cmd server now waiting for clients...");

	addrLen = sizeof(clientAddr);
	
	// Not freeing this is a nonissue
	// since it is small and we create it once
	Sessions * sessions = new Sessions;
	for(;;) {
		int connfd = accept(sockfd, (struct sockaddr*)&clientAddr, &addrLen);
		if (connfd < 0) {
			log("Failed to accept client: %04x", errno);
			break;
		}
		std::thread clientThread(clientHandler, connfd, sessions);
		clientThread.detach();
	}
	close(sockfd);
}

