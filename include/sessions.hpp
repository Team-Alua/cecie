#pragma once

#include <stdint.h>
#include <mutex>

struct ClientSession {
	char titleId[16];
	char dirName[32];
	uint64_t saveBlocks;
	char mountPath[16];
	bool reserved;
};

#define MAX_CLIENT_SESSIONS 16

class Sessions {
private:
	ClientSession sessions[MAX_CLIENT_SESSIONS];
	std::mutex mtx;

public:
	Sessions();

	/**
	 * Reserve takes a Title Id and Directory Name
	 * and checks if there are any active sessions with
	 * an identical pair. Returns the index of the reserved
	 * ClientSession instance.
	 *
	 * @param titleId
	 * @param dirName
	 * @return index of ClientSession or -1 if pair is reserved.
	 */
	int Reserve(const char * titleId, const char * dirName);


	void Free(int index);

	ClientSession * Get(int index);

	~Sessions();
};

