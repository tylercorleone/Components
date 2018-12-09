#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "commons/components_commons.h"
#include "State.h"

#include <stdint.h>
#include <stddef.h>
#include <Task.h>

class GenericDevice: public TaskManager, public Loggable {
public:
	GenericDevice(AbstractState *fallbackState = nullptr);
	GenericDevice(const char *deviceName, AbstractState *fallbackState = nullptr);
	void setup();
	void enterState(AbstractState &state);
	void enterState(AbstractState &state, const Event &event);
	void receiveEvent(const Event &event);
	virtual ~GenericDevice();
protected:
    virtual void onSetup();
private:
	void enterFallbackState();
	void enterFallbackState(const Event &event);
	AbstractState *currentState = nullptr;
	AbstractState *fallbackState = nullptr;
};

inline GenericDevice::GenericDevice(AbstractState *fallbackState) :
		fallbackState(fallbackState) {
}

inline GenericDevice::GenericDevice(const char *deviceName,
		AbstractState *fallbackState) :
		Loggable(deviceName), fallbackState(fallbackState) {
}

inline void GenericDevice::setup() {
	TaskManager::Setup();

	debugIfNamed("onSetup");

	onSetup();
}

inline void GenericDevice::onSetup() {

}

inline void GenericDevice::enterState(AbstractState &state) {
	if (currentState != nullptr) {
		debugIfOtherNamed(currentState, "onExitState");
		currentState->onExitState();
	}

	debugIfOtherNamed(&state, "onEnterState");

	if (state.onEnterState()) {
		currentState = &state;
	} else {
		warnIfOtherNamed(&state, "can't handle event");
		enterFallbackState();
	}
}

inline void GenericDevice::enterState(AbstractState &state,
		const Event &event) {
	if (currentState != nullptr) {
		debugIfOtherNamed(currentState, "onExitState");
		currentState->onExitState();
	}

	if (state.canHandleEvent(event)) {
		debugIfOtherNamed(&state, "onEnterState");
		if (state.onEnterStateWithGenericEvent(event)) {
			currentState = &state;
			return;
		}
	}

	warnIfOtherNamed(&state, "can't handle event");
	enterFallbackState(event);
}

inline void GenericDevice::receiveEvent(const Event &event) {
	if (currentState == nullptr) {
		warnIfNamed("can't handle event");
	} else {

		if (currentState->canHandleEvent(event)) {
			debugIfOtherNamed(currentState, "handleEvent");
			if (currentState->handleGenericEvent(event)) {
				return;
			}
		}

		warnIfOtherNamed(currentState, "can't handle event");
		debugIfOtherNamed(currentState, "onExitState");
		currentState->onExitState();

		enterFallbackState(event);
	}
}

inline void GenericDevice::enterFallbackState() {
	if (fallbackState != nullptr) {
		warnIfOtherNamed(fallbackState, "onEnterState");
		fallbackState->onEnterState();
		currentState = fallbackState;
	} else {
		currentState = nullptr;
		errorIfNamed("hanged up");
	}
}

inline void GenericDevice::enterFallbackState(const Event &event) {
	if (fallbackState != nullptr) {
		debugIfOtherNamed(fallbackState, "onEnterState");
		if (!(fallbackState->canHandleEvent(event)
				&& fallbackState->onEnterStateWithGenericEvent(event))) {
			fallbackState->onEnterState();
			currentState = fallbackState;
		}
	} else {
		currentState = nullptr;
		errorIfNamed("hanged up");
	}
}

inline GenericDevice::~GenericDevice() {

}

#endif
