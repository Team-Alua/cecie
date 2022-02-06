#pragma once

#include <string>
#include <unordered_map>

#include "cmd_common.hpp"
#include "cmd_list.hpp"

class CommandHandler {
public:
	void add(std::string name, cmd_function);
	bool call(std::string name, int connfd, cmd_args & args);
private:
	std::unordered_map<std::string, cmd_function> cmds;
};

void cmd_init(CommandHandler &);
