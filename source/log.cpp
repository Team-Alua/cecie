#include "log.hpp"
#include <string>
#include <stdarg.h>
#include <orbis/libkernel.h>


#define _DEBUG

#define __DEBUG_LOG_FILE

#ifdef __DEBUG_LOG_FILE
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <mutex>

#endif


// void log(const char * msg, ...) {
// 	char formatMsg[500];
// 	memset(&formatMsg, 0, sizeof(formatMsg));
// 	const char * logPrefix = "[Cecie] ";
// 	strcat(formatMsg, logPrefix);
// 	strcat(formatMsg, msg);
// 	strcat(formatMsg, "\n");
// 
// 	char outMsg[500];
// 	va_list argptr;
// 	va_start(argptr,msg);
// 	vsprintf(outMsg, formatMsg, argptr);
// 	va_end(argptr);
// 	sceKernelDebugOutText(0, outMsg);
// }

int system_notification(const char * text, const char * iconName) {
	OrbisNotificationRequest notifBuffer;
	memset(&notifBuffer, 0, sizeof(OrbisNotificationRequest));
	notifBuffer.type = OrbisNotificationRequestType::NotificationRequest;
	notifBuffer.useIconImageUri = 1;
	notifBuffer.targetId = -1;
	
	snprintf(notifBuffer.iconUri, sizeof(notifBuffer.iconUri), "cxml://psnotification/tex_%s", iconName);
	strcpy(notifBuffer.message, text);
	sceKernelSendNotificationRequest(0, &notifBuffer, 3120, 0);
	return 0;
}


#ifdef __DEBUG_LOG_FILE
std::mutex fileMtx;
#endif



void log(const char * msg, ...) {
#ifdef _DEBUG
	char formatMsg[500];
	memset(&formatMsg, 0, sizeof(formatMsg));
	strncpy(formatMsg, "[Cecie] ", sizeof("[Cecie] "));
	strncat(formatMsg, msg, strlen(msg));
	strcat(formatMsg, "\n");
	char outMsg[500];
	va_list argptr;
	va_start(argptr, msg);
	vsnprintf(outMsg, sizeof(outMsg), formatMsg, argptr);
	va_end(argptr);

#ifdef __DEBUG_LOG_FILE
	fileMtx.lock();
	do {
		FILE * logFile = fopen("/data/teamalua/log.txt", "a");
		size_t stringLen = strlen(outMsg);
		if (fwrite(outMsg, sizeof(char), stringLen, logFile) == -1) {
			// an error occured but we can't handle it.
		}
		fclose(logFile);
	} while(0);
	fileMtx.unlock();
#endif

#ifdef __DEBUG_LOG_KERNEL
	sceKernelDebugOutText(0, outMsg);
#endif

#endif
}

