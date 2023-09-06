#include "events/MessageEvent.h"
#include <string.h>

MessageEvent::MessageEvent(const char *message) :
        message(message) {}

const char *MessageEvent::getMessage() const {
    return message;
}

bool MessageEvent::equals(const char *_message) const {
    return strcmp(this->message, _message) == 0;
}

bool MessageEvent::equals(const MessageEvent &event) const {
    return equals(event.message);
}

const char *MessageEvent::getDescription() const {
    return message;
}
