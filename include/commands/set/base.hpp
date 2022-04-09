#pragma once

#include <unordered_map>

#include "commands/base.hpp"
#include "commands/events/basesystem.hpp"

class CommandSet {
protected:
	std::unordered_map<std::string, Command*> cmds;

public:
	virtual void Initialize() = 0;

	virtual const char * Name() = 0;

	Command* Get(const char * cmdName);

	void AddEventSystem(CommandEventSystem * evtSystem);
};

