#ifndef BUTTON_EVENT_H
#define BUTTON_EVENT_H

#include "Event.h"
#include <stdint.h>

class ButtonEvent : public Event {
public:
    ButtonEvent(uint8_t clicksCount, uint8_t holdStepsCount);

    const char *getDescription() const override;

    uint8_t getClicksCount() const;

    uint8_t getLongPressesCount() const;

    static ButtonEvent ofClicks(uint8_t clicksCount);

    static ButtonEvent ofLongPresses(uint8_t holdStepsCount);

    virtual ~ButtonEvent() = default;

private:
    uint8_t clicksCount;
    uint8_t holdStepsCount;
    char description[20]; // e.g. "2 clicks, 0 holds"
};

#endif