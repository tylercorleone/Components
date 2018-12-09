#ifndef EVENT_H
#define EVENT_H

/**
 * This is the base Event's type definition
 * It just defines methods for type inference
 */
class Event {
public:
	virtual const char* getEventTypeHash() const = 0;
	static bool isEventInstanceOf(Event &event) {
		return true;
	}
	virtual ~Event() {
	}
protected:
	Event() {
	}
};

#include <events/ButtonEvent.h>
#include <events/MessageEvent.h>

#endif
