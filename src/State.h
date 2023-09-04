#ifndef STATE_H
#define STATE_H

#include "commons/components_commons.h"
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

    bool enter(E *event = nullptr) {
        logActionBeginInfo("entering", event);
        bool succeeded = onEntering(event);
        if (!succeeded) {
            warnActionFail("entering", event);
        }
        return succeeded;
    }

    bool exit(E *event = nullptr) {
        logActionBeginInfo("exiting", event);
        bool succeeded = onExiting(event);
        if (!succeeded) {
            warnActionFail("exiting", event);
        }
        return succeeded;
    }

    bool handleEvent(E *event = nullptr) {
        logActionBeginInfo("handling", event);
        bool succeeded = onEventHandling(event);
        if (!succeeded) {
            warnActionFail("handling", event);
        }
        return succeeded;
    }

    virtual const char *getIdentifier() const override {
        return logger.getName();
    };

private:

    void logActionBeginInfo(const char *action, E *event) {
        logger.info("%s (event: %s)", action, event != nullptr ? event->getDescription() : "null");
    }

    void warnActionFail(const char *action, E *event) {
        logger.warn("%s failed (event: %s)", action, event != nullptr ? event->getDescription() : "null");
    }

};

#endif
