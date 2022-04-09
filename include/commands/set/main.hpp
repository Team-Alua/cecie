#pragma once

#include "commands/set/base.hpp"

class MainSet : public CommandSet {

public:
	virtual void Initialize() override;

	virtual const char * Name() override;

};
