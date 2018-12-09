#ifndef BUTTON_H
#define BUTTON_H

#include "Components.h"

#include <stdint.h>
#include <stddef.h>
#include <Task.h>

#define CONSECUTIVE_CLICKS_MAX_DELAY 600L
#define BUTTON_HOLD_BEGIN_THRESHOLD_MS 800L
#define HOLD_CYCLE_DURATION_MS 1200UL

class Button;

enum class ButtonState {
	PRESSED, RELEASED
};

class ButtonInteractionMonitor: public Task {
public:
	ButtonInteractionMonitor(Button &button, uint32_t timeInterval);
protected:
	void OnUpdate(uint32_t deltaTime) override;
private:
	Button &button;
};

class Button: public DeviceAware<GenericDevice>, public Component {
	friend class ButtonInteractionMonitor;
public:
	Button(GenericDevice &device, const char *buttonName = nullptr);
	void setState(ButtonState state);
protected:
	void onButtonFall();
	void onButtonRise();
	void reset();
	bool isUserInteracting();
	ButtonEvent ackInteraction();
	void refreshHoldStatus(uint32_t now, bool isExitingFromHold = false);
	volatile ButtonState state = ButtonState::RELEASED;
	volatile uint32_t lastFallTimeMs;
	volatile uint32_t lastRiseTimeMs;
	volatile uint8_t clicksCount;
	volatile uint8_t holdsCount;
	volatile bool haveClicksToNotify;
	volatile bool haveHoldsToNotify;
	ButtonInteractionMonitor uiMonitor { *this , MsToTaskTime(10)};
};

inline ButtonInteractionMonitor::ButtonInteractionMonitor(Button &button,
		uint32_t timeInterval) :
		Task(timeInterval), button(button) {
}

inline void ButtonInteractionMonitor::OnUpdate(uint32_t deltaTime) {
	if (button.isUserInteracting()) {

		/*
		 * User is interacting. Stay awake
		 */
		ButtonEvent event = button.ackInteraction();

		if (event.getClicksCount() > 0 || event.getHoldStepsCount() > 0) {
			button.logger.debug("%d clicks, %d holds",
					event.getClicksCount(), event.getHoldStepsCount());

			/*
			 * We have a complete interaction
			 */
			button.Device().receiveEvent(event);
		}
	} else {

		/*
		 * No interaction to intercept
		 */
		ENTER_ATOMIC_BLOCK;
		if (!button.isUserInteracting()) {

			/*
			 * Double check
			 */
			button.Device().StopTask(this);
			button.reset();
		}
		EXIT_ATOMIC_BLOCK;
	}
}

inline Button::Button(GenericDevice &device, const char *buttonName) :
		DeviceAware(device), Component(buttonName) {
	reset();
}

inline void Button::setState(ButtonState state) {
	state == ButtonState::PRESSED ? onButtonFall() : onButtonRise();
	Device().StartTask(&uiMonitor);
}

inline void Button::onButtonFall() {
	if (state == ButtonState::PRESSED) {
		logger.trace("%s filtered", "press");
		return;
	}

	logger.trace("press");

	lastFallTimeMs = MILLIS_PROVIDER();
	state = ButtonState::PRESSED;
	holdsCount = 0;
	haveHoldsToNotify = false;
}

inline void Button::onButtonRise() {
	if (state == ButtonState::RELEASED) {
		logger.trace("%s filtered", "release");
		return;
	}

	logger.trace("release");

	lastRiseTimeMs = MILLIS_PROVIDER();
	state = ButtonState::RELEASED;

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

inline ButtonEvent Button::ackInteraction() {
	uint32_t currentMs = MILLIS_PROVIDER();
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
		if (state != ButtonState::PRESSED) {
			holdsCount = 0;
		}
		haveHoldsToNotify = false;
	}

	return ButtonEvent(clicksToNotify, holdsToNotify);
}

/*
 * Used to keep alive the user interaction monitor
 */
inline bool Button::isUserInteracting() {
	if (state == ButtonState::PRESSED) {
		return true;
	} else {
		return haveClicksToNotify || haveHoldsToNotify;
	}
}

inline void Button::refreshHoldStatus(uint32_t millis, bool isExitingFromHold) {
	if (state == ButtonState::RELEASED && !isExitingFromHold) {
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

inline void Button::reset() {
	lastFallTimeMs = 0;
	lastRiseTimeMs = 0;
	clicksCount = 0;
	holdsCount = 0;
	haveClicksToNotify = false;
	haveHoldsToNotify = false;
}

#endif
