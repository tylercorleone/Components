#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <events/Event.h>

template<typename E = Event> class EventHandler {
public:
	virtual bool handleEvent(const E &event) = 0;
	virtual ~EventHandler() {}
protected:
	bool canHandleEvent(const Event &event) {
		return E::isEventInstanceOf(event);
	}
};

#endif
