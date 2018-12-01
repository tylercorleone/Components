#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "commons/components_commons.h"
#include "State.h"

#include <stdint.h>
#include <stddef.h>
#include <Task.h>

class GenericDevice: public TaskManager, public Named {
public:
	GenericDevice(AbstractState *fallbackState = nullptr);
	GenericDevice(const char *deviceName, AbstractState *fallbackState = nullptr);
	void enterState(AbstractState &state);
	void enterState(AbstractState &state, const GenericEvent &event);
	void receiveEvent(const GenericEvent &event);
	void Setup();
protected:
    virtual void onSetup();
private:
	void enterFallbackState();
	void enterFallbackState(const GenericEvent &event);
	AbstractState *currentState = nullptr;
	AbstractState *fallbackState = nullptr;
};

inline GenericDevice::GenericDevice(AbstractState *fallbackState) :
		fallbackState(fallbackState) {Setup();
}

inline GenericDevice::GenericDevice(const char *deviceName,
		AbstractState *fallbackState) :
		Named(deviceName), fallbackState(fallbackState) {
}

#define debugIfStateHasName debugIfOtherNamed

inline void GenericDevice::enterState(AbstractState &state) {
	if (currentState != nullptr) {
		debugIfStateHasName(currentState, "onExitState");
		currentState->onExitState();
	}

	debugIfStateHasName(&state, "onEnterState");

	if (state.onEnterState()) {
		currentState = &state;
	} else {
		enterFallbackState();
	}
}

inline void GenericDevice::enterState(AbstractState &state, const GenericEvent &event) {
	if (currentState != nullptr) {
		debugIfStateHasName(currentState, "onExitState");
		currentState->onExitState();
	}

	if (state.canHandleEvent(event)) {
		debugIfStateHasName(&state, "onEnterState");
		if (state.onEnterStateWithGenericEvent(event)) {
			currentState = &state;
		} else {
			enterFallbackState(event);
		}
	} else {
		debugIfStateHasName(&state, "can't handle event");
		enterFallbackState(event);
	}
}

inline void GenericDevice::receiveEvent(const GenericEvent &event) {
	if (currentState == nullptr) {
		debugIfNamed("can't handle event");
		return;
	}

	if (currentState->canHandleEvent(event)) {
		debugIfStateHasName(currentState, "handleEvent");
		if (!currentState->handleGenericEvent(event)) {
			debugIfStateHasName(currentState, "onExitState");
			currentState->onExitState();
			enterFallbackState();
		}
	} else {
		debugIfStateHasName(currentState, "can't handle event");
		debugIfStateHasName(currentState, "onExitState");
		currentState->onExitState();
		enterFallbackState(event);
	}
}

inline void GenericDevice::enterFallbackState() {
	if (fallbackState != nullptr) {
		debugIfStateHasName(fallbackState, "onEnterState");
		fallbackState->onEnterState();
		currentState = fallbackState;
	} else {
		currentState = nullptr;
		debugIfNamed("hanged up");
	}
}

inline void GenericDevice::enterFallbackState(const GenericEvent &event) {
	if (fallbackState != nullptr) {
		debugIfStateHasName(fallbackState, "onEnterState");
		if (!(fallbackState->canHandleEvent(event)
				&& fallbackState->onEnterStateWithGenericEvent(event))) {
			fallbackState->onEnterState();
			currentState = fallbackState;
		}
	} else {
		currentState = nullptr;
		debugIfNamed("hanged up");
	}
}

inline void GenericDevice::Setup() {
	debugIfNamed("setup");

	TaskManager::Setup();
	onSetup();
}

inline void GenericDevice::onSetup() {

}

#undef debugIfStateHasName

#endif
