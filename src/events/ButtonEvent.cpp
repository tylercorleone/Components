#include <events/ButtonEvent.h>
#include <stdio.h>

ButtonEvent::ButtonEvent(uint8_t clicksCount, uint8_t holdStepsCount) :
        clicksCount(clicksCount),
        holdStepsCount(holdStepsCount),
        description("\n") {

    snprintf(description, 20, "%d clicks, %d holds", clicksCount, holdStepsCount);
}

uint8_t ButtonEvent::getClicksCount() const {
    return clicksCount;
}

uint8_t ButtonEvent::getLongPressesCount() const {
    return holdStepsCount;
}

const char *ButtonEvent::getDescription() const {
    return description;
}

ButtonEvent ButtonEvent::ofClicks(uint8_t clicksCount) {
    return {clicksCount, 0};
}

ButtonEvent ButtonEvent::ofLongPresses(uint8_t holdStepsCount) {
    return {0, holdStepsCount};
}