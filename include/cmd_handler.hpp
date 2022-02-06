#pragma once

#include <string>
#include <unordered_map>

#include "cmd_common.hpp"
#include "cmd_list.hpp"

const int CALL_CMD_UNKNOWN = 0;
const int CALL_CMD_FAILED = 1;
const int CALL_CMD_SUCESS = 2;

class CommandHandler {
public:
	void add(std::string name, cmd_function);
	int call(std::string name, int connfd, cmd_args & args);
private:
	std::unordered_map<std::string, cmd_function> cmds;
};

void cmd_init(CommandHandler &);
