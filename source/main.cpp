#include "main.hpp"
#include <orbis/libkernel.h>
#include <orbis/UserService.h>
#include <orbis/SaveData.h>

int initModules() {
	OrbisUserServiceInitializeParams params;
	memset(&params, 0, sizeof(OrbisUserServiceInitializeParams));
	params.priority = 700;
	int32_t userServiceResult = sceUserServiceInitialize(&params);

	if (userServiceResult != 0) {
		log("Failed to initialize user service: %d", userServiceResult);
		return -1;
	}

	int32_t saveDataInitializeResult = sceSaveDataInitialize3(0);

	if (saveDataInitializeResult != 0) {
		log("Failed to initialize save data library: %d", saveDataInitializeResult);
		return -1;
	}
	return 0;
}

bool (*jailbreak)();

int resolveDynamicLinks() {
	int libjbc = sceKernelLoadStartModule("/app0/sce_module/libjbc.sprx", 0, NULL, 0, NULL, NULL);
	if (libjbc == 0) {
		log("Failed to load module %s", "libjbc.sprx");
		return -1;
	}
	sceKernelDlsym(libjbc, "Jailbreak", (void**)&jailbreak);
	return 0;
}

int main() {
	setvbuf(stdout, NULL, _IONBF, 0);
	do {
		if (initModules() != 0) {
			break;
		}
		if (resolveDynamicLinks() != 0) {
			break;
		}
		if (!jailbreak()) {
			log("Failed to escape sandbox");
			break;
		}
		log("Ready to initiate server threads.");
		std::thread pongThread(pongServer);
		pongThread.detach();
	
		std::thread cmdServerThread(cmdServer);
		cmdServerThread.detach();
	} while(0);

	INF_LOOP
}

