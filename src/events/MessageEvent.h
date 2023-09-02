#ifndef MESSAGE_EVENT_H
#define MESSAGE_EVENT_H

#include <events/Event.h>
#include <string.h>

class MessageEvent : public Event {
public:
    MessageEvent(const char *message);

    const char *getMessage() const;

    bool equals(const char *message) const;

    bool equals(const MessageEvent &event) const;

    virtual const char *getDescription() const override;

    virtual ~MessageEvent();

private:
    static const char *MessageEventTypeHash();

    const char *message;
};

inline MessageEvent::MessageEvent(const char *message) :
        message(message) {
}

inline const char *MessageEvent::getMessage() const {
    return message;
}

inline bool MessageEvent::equals(const char *message) const {
    return strcmp(this->message, message) == 0;
}

inline bool MessageEvent::equals(const MessageEvent &event) const {
    return equals(event.message);
}

inline const char *MessageEvent::getDescription() const {
    return message;
}

inline MessageEvent::~MessageEvent() {

}

#endif
