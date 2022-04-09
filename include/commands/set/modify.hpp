#pragma once

#include "commands/set/base.hpp"

class ModifySet : public CommandSet {

public:
	virtual void Initialize() override;

	virtual const char * Name() override;
};
