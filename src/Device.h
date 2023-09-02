#ifndef DEVICE_H
#define DEVICE_H

#include "commons/components_commons.h"
#include "State.h"

#include <stdint.h>
#include <stddef.h>
#include <Task.h>
#include <type_traits>

template<typename E>
class Device : public Identifiable {
public:
    Device(State<E> &initialState,
           const char *name,
           LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void initialize();

    void loop(uint16_t watchdogTimeOutFlag = WDTO_500MS);

    /**
     * Enables components to schedule activities.
     */
    TaskManager &getTaskManager();

    void enterState(State<E> &state, E *event = nullptr);

    void receiveEvent(E *event);

    virtual const char *getIdentifier() const {
        return logger.getName();
    }

    virtual ~Device() {}

protected:
    virtual void onInitialize() {};

    virtual void onIdle() {};

    virtual void enterInitialState(E *event);

    State<E> &initialState;
    State<E> *currentState;
    TaskManager taskManager;
    Logger logger;
};

template<typename E>
inline Device<E>::Device(State<E> &initialState,
                         const char *name,
                         LogLevel logLevel) :
        initialState(initialState),
        currentState(&initialState),
        logger(Logger(name, logLevel)) {}

template<typename E>
inline void Device<E>::initialize() {
    logger.debug("initializing");
    onInitialize(); // customizable initial set up
    initialState.enter();
}

template<typename E>
inline void Device<E>::loop(uint16_t watchdogTimeOutFlag) {
    taskManager.Loop(watchdogTimeOutFlag);

    if (taskManager.IsIdle()) {
#ifdef DEBUG_ENTERING_IDLE_STATE
        logger.debug("entering idle");
#endif
        onIdle();
    }
}

template<typename E>
inline TaskManager &Device<E>::getTaskManager() {
    return taskManager;
}

template<typename E>
inline void Device<E>::enterState(State<E> &state, E *event) {
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

#endif
