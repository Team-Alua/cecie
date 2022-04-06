
#include "log.hpp"
#include <string>
#include <stdarg.h>
#include <orbis/libkernel.h>

void log(const char * msg, ...) {
	char formatMsg[500];
	memset(&formatMsg, 0, sizeof(formatMsg));
	const char * logPrefix = "[Cecie] ";
	strcat(formatMsg, logPrefix);
	strcat(formatMsg, msg);
	strcat(formatMsg, "\n");

	char outMsg[500];
	va_list argptr;
	va_start(argptr,msg);
	vsprintf(outMsg, formatMsg, argptr);
	va_end(argptr);
	sceKernelDebugOutText(0, outMsg);
}

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
