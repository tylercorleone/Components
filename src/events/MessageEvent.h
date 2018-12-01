#ifndef MESSAGEEVENT_H
#define MESSAGEEVENT_H

#include "GenericEvent.h"
#include <string.h>

class MessageEvent: public GenericEvent {
public:
	MessageEvent(const char *message);
	const char* getMessage() const;
	bool equals(const char *message) const;
	bool equals(const MessageEvent &event) const;
	virtual const char* getEventTypeHash() const override;
	static bool isEventInstanceOf(const GenericEvent &event);
	virtual ~MessageEvent();
private:
	static const char* MessageEventTypeHash();
	const char *message;
};

inline MessageEvent::MessageEvent(const char *message) :
		message(message) {
}

inline const char* MessageEvent::getMessage() const {
	return message;
}

inline bool MessageEvent::equals(const char *message) const {
	return strcmp(this->message, message) == 0;
}

inline bool MessageEvent::equals(const MessageEvent &event) const {
	return strcmp(this->message, event.message) == 0;
}

inline const char* MessageEvent::getEventTypeHash() const {
	return MessageEvent::MessageEventTypeHash();
}

inline bool MessageEvent::isEventInstanceOf(const GenericEvent &event) {
	return MessageEvent::MessageEventTypeHash() == event.getEventTypeHash();
}

inline const char* MessageEvent::MessageEventTypeHash() {
	const static char staticVar = 'M';
	return &staticVar;
}

inline MessageEvent::~MessageEvent() {

}

#endif
