#ifndef MESSAGE_EVENT_H
#define MESSAGE_EVENT_H

#include "events/Event.h"

class MessageEvent : public Event {
public:
    explicit MessageEvent(const char *message);

    const char *getDescription() const override;

    const char *getMessage() const;

    bool equals(const char *message) const;

    bool equals(const MessageEvent &event) const;

    virtual ~MessageEvent() = default;

private:
    const char *message;
};

#endif
