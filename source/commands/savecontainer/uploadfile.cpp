#include "commands/savecontainer/uploadfile.hpp"

#include "commands/events/names.hpp"
#include "commands/events/phases.hpp"
#include "constants.hpp"

#include "filesystem.hpp"

void UploadFileToSaveContainerCommand::Execute(Network & network, int & sessionIndex, Sessions & sessions) {
	ClientSession * clientSession = sessions.Get(sessionIndex);

	FileMetadata metadata;

	ssize_t metaDataResult  = network.readFull(&metadata);
	if (metaDataResult == -1) {
		// log this error
		CommandSystemStates state = CommandSystemStates::Done;
		this->Notify(EventNamePhaseChange, &state);
		return;
	}
	char mountFolder[256];
	memset(mountFolder, 0, sizeof(mountFolder));
	sprintf(mountFolder, "/mnt/sandbox/%s_000%s/%s", TITLE_ID, clientSession->mountPath,  metadata.path);
	ssize_t downloadResult = network.downloadFile(mountFolder, metadata.size);
	if (downloadResult == -1) {
		// Log this error too
		CommandSystemStates state = CommandSystemStates::Done;
		this->Notify(EventNamePhaseChange, &state);
		return;
	}
	network.sendResponse("ok");
}

