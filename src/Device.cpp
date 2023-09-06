#include "Device.h"
#include "Components.h"

template<typename E>
inline Device<E>::Device(State <E> &initialState,
                         const char *name,
                         LogLevel logLevel) :
        initialState(initialState),
        currentState(&initialState),
        logger(Logger(name, logLevel)) {}

template<typename E>
inline const char *Device<E>::getIdentifier() const {
    return logger.getName();
}

template<typename E>
inline void Device<E>::initialize() {
    logger.info("initializing");
    onInitialize(); // customizable initial set up
    initialState.enter();
}

template<typename E>
inline void Device<E>::loop(uint16_t watchdogTimeOutFlag) {
    taskManager.Loop(watchdogTimeOutFlag);

    if (taskManager.IsIdle()) {
        if (DEBUG_ENTERING_IDLE_STATE) {
            logger.debug("entering idle");
        }
        onIdle();
    }
}

template<typename E>
inline TaskManager &Device<E>::getTaskManager() {
    return taskManager;
}

template<typename E>
inline void Device<E>::enterState(State <E> &state, E *event) {
    if (!currentState->exit(event)) {
        enterInitialState(event);
        return;
    }

    if (!state.enter(event)) {
        enterInitialState(event);
        return;
    }

    currentState = &state;
}

template<typename E>
inline void Device<E>::receiveEvent(E *event) {
    if (!currentState->handleEvent(event)) {
        enterInitialState(event);
    }
}

template<typename E>
inline void Device<E>::enterInitialState(E *event) {
    logger.warn("entering initial state");
    initialState.enter(event);
    currentState = &initialState;
}
