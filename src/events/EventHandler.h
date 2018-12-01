#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include "GenericEvent.h"

template<typename E = GenericEvent> class EventHandler {
public:
	virtual bool handleEvent(const E &event) = 0;
	virtual ~EventHandler() {}
protected:
	bool canHandleEvent(const GenericEvent &event) {
		return E::isEventInstanceOf(event);
	}
};

#endif
