#include "sessions.hpp"
#include <string.h>

Sessions::Sessions() {
	for(int i = 0; i < MAX_CLIENT_SESSIONS; i++) {
		sessions[i].reserved = false;
	}
}
  
Sessions::~Sessions() {

}

int Sessions::Reserve(const char * titleId, const char * dirName) {
	int index = -1;

	mtx.lock();
	do {
		bool free = true;
		for(int i = 0; i < MAX_CLIENT_SESSIONS; i++) {
			struct ClientSession session = sessions[i];
			if (!session.reserved) {
				continue;
			}
	
			if (strcmp(session.titleId, titleId)) {
				continue;
			}
	
			if (strcmp(session.dirName, dirName)) {
				continue;
			}
			free = false;
			break;
		}
		if (!free) {
			break;
		}
		for(int i = 0; i < MAX_CLIENT_SESSIONS; i++) {
			if (!sessions[i].reserved) {
				sessions[i].reserved = true;
				strcpy(sessions[i].titleId, titleId);
				strcpy(sessions[i].dirName, dirName);
				index = i;
				break;
			}
		}
	}  while(false);
	mtx.unlock();
	return index;
}

void Sessions::Free(int index) {
	if (index >=  MAX_CLIENT_SESSIONS || index < 0) {
		return;
	}

	mtx.lock();
	sessions[index].reserved = false;
	mtx.unlock();
}

ClientSession * Sessions::Get(int index) {
	if (index >=  MAX_CLIENT_SESSIONS || index < 0) {
		return NULL;
	}
	return &sessions[index];
}

