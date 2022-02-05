#include "pong.hpp"
#include "common.hpp"
static void pongClientHandler(int connfd);

void pongServer() {
	log("Inside of pongServer");
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
	serverAddr.sin_port = htons(PONG_PORT); // TODO: Do not hardcode this
	
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
		std::thread clientThread(pongClientHandler, connfd);
		clientThread.detach();
	}
	close(sockfd);
}

static void pongClientHandler(int connfd) {
	log("Client connected!");
	while(true) {
		uint32_t message = 0;
		int bytesRead = read(connfd, &message , 4);
		if (bytesRead < 0) {
			log("Client likely disconnected", errno);
			close(connfd);
			return;
		}
		// ping in number form as little or big endian
		if (message != 1735289200 && message != 1885957735) {
			log("Client sent a weird message: %04x", message);
			close(connfd);
			return;
		}
		// pong as a little endian number 
		// Convert this into a byte array
		// So endianness doesn't really matter
		uint32_t response = 1735290736;
		write(connfd, &response, sizeof(response));
		log("Sent response");
	}
}

