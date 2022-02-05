#include "common.hpp"
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
