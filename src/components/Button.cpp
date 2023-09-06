#include "components/Button.h"

ButtonInteractionMonitor::ButtonInteractionMonitor(Button &button,
                                                          uint32_t timeInterval) :
        Task(timeInterval), button(button) {
}

void ButtonInteractionMonitor::OnUpdate(uint32_t deltaTime) {
    if (button.isUserInteracting()) {

        /*
         * User is interacting. Stay awake
         */
        ButtonEvent event = button.ackInteraction();

        if (event.getClicksCount() > 0 || event.getLongPressesCount() > 0) {
            button.logger.debug(event.getDescription());

            /*
             * We have a complete interaction
             */
            button.getDevice().receiveEvent(&event);
        }
    } else {

        /*
         * No interaction to intercept
         */
        noInterrupts();
        if (!button.isUserInteracting()) {

            /*
             * Double check
             */
            button.getDevice().getTaskManager().StopTask(this);
            button.reset();
        }
        interrupts();
    }
}

Button::Button(Device<ButtonEvent> &device,
                      const char *name,
                      LogLevel logLevel) :
        DeviceAware(device),
        Component(name, logLevel) {
    reset();
}

void Button::setState(ButtonState state) {
    state == ButtonState::PRESSED ? onButtonFall() : onButtonRise();
    getDevice().getTaskManager().StartTask(&uiMonitor);
}

void Button::onButtonFall() {
    if (currentState == ButtonState::PRESSED) {
        logger.trace("%s filtered", "pressed");
        return;
    }

    logger.trace("pressed");

    lastFallTimeMs = millis();
    currentState = ButtonState::PRESSED;
    holdsCount = 0;
    haveHoldsToNotify = false;
}

void Button::onButtonRise() {
    if (currentState == ButtonState::RELEASED) {
        logger.trace("%s filtered", "released");
        return;
    }

    logger.trace("released");

    lastRiseTimeMs = millis();
    currentState = ButtonState::RELEASED;

    if (lastRiseTimeMs - lastFallTimeMs >= BUTTON_HOLD_BEGIN_THRESHOLD_MS) {

        /*
         * Exiting from a hold
         */
        refreshHoldStatus(lastRiseTimeMs, true);
    } else {

        /*
         * This is a click
         */
        ++clicksCount;
        haveClicksToNotify = true;
    }
}

ButtonEvent Button::ackInteraction() {
    uint32_t currentMs = millis();
    uint8_t clicksToNotify = 0;

    if (haveClicksToNotify) {
        if (currentMs - lastRiseTimeMs < CONSECUTIVE_CLICKS_MAX_DELAY) {

            /*
             * There is still time to collect clicks, so we cannot notify them yet
             */
            clicksToNotify = 0;
        } else {
            clicksToNotify = clicksCount;
            clicksCount = 0;
            haveClicksToNotify = false;
        }
    }

    refreshHoldStatus(currentMs);
    uint8_t holdsToNotify = 0;

    if (haveHoldsToNotify) {
        holdsToNotify = holdsCount;
        if (currentState != ButtonState::PRESSED) {
            holdsCount = 0;
        }
        haveHoldsToNotify = false;
    }

    return ButtonEvent(clicksToNotify, holdsToNotify);
}

/*
 * Used to keep alive the user interaction monitor
 */
bool Button::isUserInteracting() {
    return currentState == ButtonState::PRESSED || haveClicksToNotify
           || haveHoldsToNotify;
}

void Button::refreshHoldStatus(uint32_t millis, bool isExitingFromHold) {
    if (currentState == ButtonState::RELEASED && !isExitingFromHold) {
        return;
    }

    uint32_t holdDurationMs = millis - lastFallTimeMs;

    if (holdDurationMs < BUTTON_HOLD_BEGIN_THRESHOLD_MS) {

        /*
         * Too soon
         */
        return;
    }

    uint8_t holdsCountTemp;
    holdsCountTemp = (holdDurationMs - BUTTON_HOLD_BEGIN_THRESHOLD_MS)
                     / HOLD_CYCLE_DURATION_MS + 1;

    if (holdsCountTemp == holdsCount) {

        /*
         * Already notified
         */
        return;
    }

    holdsCount = holdsCountTemp;
    haveHoldsToNotify = true;
}

void Button::reset() {
    lastFallTimeMs = 0;
    lastRiseTimeMs = 0;
    clicksCount = 0;
    holdsCount = 0;
    haveClicksToNotify = false;
    haveHoldsToNotify = false;
}
