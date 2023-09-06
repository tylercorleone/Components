#include "State.h"

template<typename E>
bool State<E>::enter(E *event) {
    logActionBeginInfo("entering", event);
    bool succeeded = onEntering(event);
    if (!succeeded) {
        warnActionFail("entering", event);
    }
    return succeeded;
}

template<typename E>
bool State<E>::exit(E *event) {
    logActionBeginInfo("exiting", event);
    bool succeeded = onExiting(event);
    if (!succeeded) {
        warnActionFail("exiting", event);
    }
    return succeeded;
}

template<typename E>
bool State<E>::handleEvent(E *event) {
    logActionBeginInfo("handling", event);
    bool succeeded = onEventHandling(event);
    if (!succeeded) {
        warnActionFail("handling", event);
    }
    return succeeded;
}

template<typename E>
const char *State<E>::getIdentifier() const {
    return logger.getName();
}

template<typename E>
void State<E>::logActionBeginInfo(const char *action, E *event) {
    logger.info("%s (event: %s)", action, event != nullptr ? event->getDescription() : "null");
}

template<typename E>
void State<E>::warnActionFail(const char *action, E *event) {
    logger.warn("%s failed (event: %s)", action, event != nullptr ? event->getDescription() : "null");
}
