#pragma once
#include "commands/events/event.hpp"

typedef void (*EventCallback)(Event& event, void * instance);

struct EventCallbackPair {
	EventCallback cb;
	void * instance;
};

