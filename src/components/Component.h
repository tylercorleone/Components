#ifndef COMPONENT_H
#define COMPONENT_H

#include "Components.h"

class Component : public Identifiable {
public:
    virtual const char *getIdentifier() const override {
        return logger.getName();
    };
    Logger logger;
protected:

    Component(const char *name,
              LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            logger(Logger(name, logLevel)) {}
};

#endif