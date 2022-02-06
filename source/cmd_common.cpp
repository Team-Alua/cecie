
#include <orbis/UserService.h>

#include "cmd_common.hpp"

int32_t getUserId() {
	int32_t outUserId;
	sceUserServiceGetInitialUser(&outUserId);
	return outUserId;
}
