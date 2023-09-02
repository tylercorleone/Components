#ifndef BUTTON_EVENT_H
#define BUTTON_EVENT_H

#include <events/Event.h>
#include <stdint.h>
#include <stdio.h>

class ButtonEvent : public Event {
public:
    ButtonEvent(uint8_t clicksCount, uint8_t holdStepsCount);

    uint8_t getClicksCount() const;

    uint8_t getLongPressesCount() const;

    virtual const char *getDescription() const override;

    virtual ~ButtonEvent() {}

    static ButtonEvent ofClicks(uint8_t clicksCount) {
        return ButtonEvent(clicksCount, 0);
    }

    static ButtonEvent ofLongPresses(uint8_t holdStepsCount) {
        return ButtonEvent(0, holdStepsCount);
    }

private:
    static const char *ButtonEventTypeHash();

    uint8_t clicksCount;
    uint8_t holdStepsCount;
    char description[20]; // e.g. "2 clicks, 0 holds"
};

inline ButtonEvent::ButtonEvent(uint8_t clicksCount, uint8_t holdStepsCount) :
        clicksCount(clicksCount),
        holdStepsCount(holdStepsCount) {
    snprintf(description, 20, "%d clicks, %d holds", clicksCount, holdStepsCount);
}

inline uint8_t ButtonEvent::getClicksCount() const {
    return clicksCount;
}

inline uint8_t ButtonEvent::getLongPressesCount() const {
    return holdStepsCount;
}

inline const char *ButtonEvent::getDescription() const {
    return description;
}

#endif
