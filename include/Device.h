#ifndef DEVICE_H
#define DEVICE_H

#include "Identifiable.h"
#include "State.h"
#include "Logger.h"
#include "task_includer.h"
#include <stdint.h>

template<typename E>
class Device : public Identifiable {
public:
    explicit Device(State<E> &initialState,
           const char *name = "Device",
           LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void initialize();

    void loop(uint16_t watchdogTimeOutFlag = WDTO_500MS);

    /**
     * Enables components to schedule activities.
     */
    TaskManager &getTaskManager();

    void enterState(State <E> &state, E *event = nullptr);

    void receiveEvent(E *event);

    virtual const char *getIdentifier() const override;

    virtual ~Device() {}

protected:
    virtual void onInitialize() {};

    virtual void onIdle() {};

    virtual void enterInitialState(E *event);

    State <E> &initialState;
    State <E> *currentState;
    TaskManager taskManager;
    Logger logger;
};

#endif
