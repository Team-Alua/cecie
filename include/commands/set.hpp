#pragma once

#include <unordered_map>
#include <memory>

#include "commands/base.hpp"
#include "commands/events/basesystem.hpp"

class CommandSet {
protected:
	std::unordered_map<const char *, std::shared_ptr<Command>> cmds;

public:
	virtual void Initialize() = 0;

	std::shared_ptr<Command> Get(const char * cmdName);

	void AddEventSystem(CommandEventSystem * evtSystem);
};

