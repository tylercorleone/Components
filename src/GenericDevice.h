#ifndef GENERICDEVICE_H
#define GENERICDEVICE_H

#include "commons/components_commons.h"
#include "State.h"

#include <stdint.h>
#include <stddef.h>
#include <Task.h>

class GenericDevice: public TaskManager {
public:
	GenericDevice(AbstractState *fallbackState = nullptr);
	GenericDevice(AbstractState *fallbackState, const char *deviceName,
			LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);
	void setup();
	void enterState(AbstractState &state);
	void enterState(AbstractState &state, const Event &event);
	void receiveEvent(const Event &event);
	virtual ~GenericDevice();
protected:
    virtual void onSetup();
	Logger logger {nullptr, LogLevel::OFF};
private:
	void enterFallbackState();
	void enterFallbackState(const Event &event);
	AbstractState *currentState = nullptr;
	AbstractState *fallbackState = nullptr;
};

inline GenericDevice::GenericDevice(AbstractState *fallbackState) :
		fallbackState(fallbackState) {
}

inline GenericDevice::GenericDevice(AbstractState *fallbackState,
		const char *deviceName, LogLevel logLevel) :
		fallbackState(fallbackState) {
	logger.setName(deviceName);
	logger.setLogLevel(logLevel);
}

inline void GenericDevice::setup() {
	TaskManager::Setup();

	logger.debug("onSetup");
	onSetup();
}

inline void GenericDevice::onSetup() {

}

inline void GenericDevice::enterState(AbstractState &state) {
	if (currentState != nullptr) {
		currentState->logger.debug("onExitState");
		currentState->onExitState();
	}

	currentState->logger.debug("onEnterState");

	if (state.onEnterState()) {
		currentState = &state;
	} else {
		currentState->logger.warn("can't handle event");
		enterFallbackState();
	}
}

inline void GenericDevice::enterState(AbstractState &state,
		const Event &event) {
	if (currentState != nullptr) {
		currentState->logger.debug("onExitState");
		currentState->onExitState();
	}

	if (state.canHandleEvent(event)) {
		currentState->logger.debug("onEnterState");
		if (state.onEnterStateWithGenericEvent(event)) {
			currentState = &state;
			return;
		}
	}

	currentState->logger.warn("can't handle event");
	enterFallbackState(event);
}

inline void GenericDevice::receiveEvent(const Event &event) {
	if (currentState == nullptr) {
		currentState->logger.warn("can't handle event");
	} else {

		if (currentState->canHandleEvent(event)) {
			currentState->logger.debug("handleEvent");
			if (currentState->handleGenericEvent(event)) {
				return;
			}
		}

		currentState->logger.warn("can't handle event");
		currentState->logger.debug("onExitState");
		currentState->onExitState();

		enterFallbackState(event);
	}
}

inline void GenericDevice::enterFallbackState() {
	if (fallbackState != nullptr) {
		fallbackState->logger.warn("onEnterState");
		fallbackState->onEnterState();
		currentState = fallbackState;
	} else {
		currentState = nullptr;
		logger.error("hanged up");
	}
}

inline void GenericDevice::enterFallbackState(const Event &event) {
	if (fallbackState != nullptr) {
		fallbackState->logger.warn("onEnterState");
		if (!(fallbackState->canHandleEvent(event)
				&& fallbackState->onEnterStateWithGenericEvent(event))) {
			fallbackState->onEnterState();
			currentState = fallbackState;
		}
	} else {
		currentState = nullptr;
		logger.error("hanged up");
	}
}

inline GenericDevice::~GenericDevice() {

}

#endif
