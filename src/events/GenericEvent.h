#ifndef GENERICEVENT_H
#define GENERICEVENT_H

#include <stdint.h>

class GenericEvent {
	/*
	 * Does nothing, just type inference
	 */
public:
	virtual const char* getEventTypeHash() const = 0;
	static bool isEventInstanceOf(GenericEvent &event) {return true;}
	virtual ~GenericEvent() {}
protected:
	GenericEvent() {}
};

#include <events/ButtonEvent.h>
#include <events/MessageEvent.h>

#endif
