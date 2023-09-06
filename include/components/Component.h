#ifndef COMPONENT_H
#define COMPONENT_H

#include "Identifiable.h"
#include "Logger.h"

class Component : public Identifiable {
public:
    const char *getIdentifier() const override;

    Logger logger;
protected:
    explicit Component(const char *name, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);
};

#endif
