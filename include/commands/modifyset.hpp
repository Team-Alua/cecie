#pragma once

#include "commands/set.hpp"

class ModifySet : public CommandSet {

public:
	virtual void Initialize() override;

	virtual const char * Name() override;
};
