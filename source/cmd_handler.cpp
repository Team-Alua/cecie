#include "cmd_handler.hpp"

void CommandHandler::add(std::string name, cmd_function func) {
	cmds[name] = func;
}

bool CommandHandler::call(std::string name, int connfd, cmd_args & args) {
	auto entry = cmds.find(name);
	if (entry == cmds.end()) {
		return false;
	}
	entry->second(connfd, args);
	return true;
}

//seg:cmd_init

void cmd_init(CommandHandler & handler) {
	handler.add("TouchSave", touch_save);
}
