#ifndef STATE_H
#define STATE_H

#include "Identifiable.h"
#include "Logger.h"
#include "DeviceAware.h"

template<typename E>
class State : public Identifiable {
protected:
    State(const char *name, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            logger(name, logLevel) {}

    virtual bool onEntering(E *event = nullptr) { return true; }

    virtual bool onExiting(E *event = nullptr) { return true; }

    virtual bool onEventHandling(E *event = nullptr) { return true; }

    virtual ~State() {};

    Logger logger;

public:

    bool enter(E *event = nullptr);

    bool exit(E *event = nullptr);

    bool handleEvent(E *event = nullptr);

    virtual const char *getIdentifier() const override;

private:

    void logActionBeginInfo(const char *action, E *event);

    void warnActionFail(const char *action, E *event);

};

#endif
