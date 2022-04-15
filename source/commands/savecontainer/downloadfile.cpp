#include "commands/savecontainer/downloadfile.hpp"

#include "commands/events/names.hpp"
#include "commands/events/phases.hpp"

#include "constants.hpp"

#include "filesystem.hpp"

void DownloadFileFromSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);

	FileMetadata upload;
	ssize_t fileRequestRead = network.readFull(&upload);
	if (fileRequestRead == -1) {
		// log error
		CommandSystemStates state = CommandSystemStates::Done;
		this->Notify(EventNamePhaseChange, &state);
		return;
	}

	char mountFolder[256];
	memset(mountFolder, 0, sizeof(mountFolder));
	sprintf(mountFolder, "/mnt/sandbox/%s_000%s/%s", TITLE_ID, clientSession->mountPath,  upload.path);
	ssize_t uploadResult = network.uploadFile(mountFolder, upload.size);
	if (uploadResult == -1) {
		// log error
		CommandSystemStates state = CommandSystemStates::Done;
		this->Notify(EventNamePhaseChange, &state);
		return;
	}
	network.sendResponse("ok");
}
