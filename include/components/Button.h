#ifndef BUTTON_H
#define BUTTON_H

#include "Component.h"
#include "DeviceAware.h"
#include "Device.h"
#include "events/ButtonEvent.h"

#ifndef BUTTON_MONITOR_TIME_INTERVAL_MS
#define BUTTON_MONITOR_TIME_INTERVAL_MS 10
#endif

#define SPEED_FAST 3
#define SPEED_NORMAL 2
#define SPEED_SLOW 1

#ifndef BUTTON_INTERACTION_SPEED
#define BUTTON_INTERACTION_SPEED SPEED_NORMAL
#endif

#if BUTTON_INTERACTION_SPEED == SPEED_FAST
#define CONSECUTIVE_CLICKS_MAX_DELAY 450L
#define BUTTON_HOLD_BEGIN_THRESHOLD_MS 600L
#define HOLD_CYCLE_DURATION_MS 900UL
#elif BUTTON_INTERACTION_SPEED == SPEED_NORMAL
#define CONSECUTIVE_CLICKS_MAX_DELAY 600L
#define BUTTON_HOLD_BEGIN_THRESHOLD_MS 800L
#define HOLD_CYCLE_DURATION_MS 1200UL
#elif BUTTON_INTERACTION_SPEED == SPEED_SLOW
#define CONSECUTIVE_CLICKS_MAX_DELAY 750L
#define BUTTON_HOLD_BEGIN_THRESHOLD_MS 1000L
#define HOLD_CYCLE_DURATION_MS 1500UL
#endif

class Button;

enum class ButtonState {
    PRESSED, RELEASED
};

class ButtonInteractionMonitor : public Task {
public:
    ButtonInteractionMonitor(Button &button, uint32_t timeInterval);

protected:
    void OnUpdate(uint32_t deltaTime) override;

private:
    Button &button;
};

class Button : public DeviceAware<Device<ButtonEvent>>, public Component {
    friend class ButtonInteractionMonitor;

public:
    Button(Device<ButtonEvent> &device,
           const char *name = "button",
           LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void setState(ButtonState state);

protected:
    void onButtonFall();

    void onButtonRise();

    void reset();

    bool isUserInteracting();

    ButtonEvent ackInteraction();

    void refreshHoldStatus(uint32_t now, bool isExitingFromHold = false);

    volatile ButtonState currentState = ButtonState::RELEASED;
    volatile uint32_t lastFallTimeMs;
    volatile uint32_t lastRiseTimeMs;
    volatile uint8_t clicksCount;
    volatile uint8_t holdsCount;
    volatile bool haveClicksToNotify;
    volatile bool haveHoldsToNotify;
    ButtonInteractionMonitor uiMonitor{*this, MsToTaskTime(
            BUTTON_MONITOR_TIME_INTERVAL_MS)};
};

#endif
