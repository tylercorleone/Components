#include "components/Component.h"

Component::Component(const char *name, LogLevel logLevel) :
        logger(name, logLevel) {}

const char *Component::getIdentifier() const {
    return logger.getName();
}
