#ifndef BATTERY_H
#define BATTERY_H

#include "components/Component.h"

/**
 * Battery is a component that can be inspected
 * to get the remaining charge level
 */
class Battery : public Component {
public:
    explicit Battery(const char *name = "battery",
                     LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Component(name, logLevel) {}

    virtual float getRemainingCharge() const = 0;

    virtual ~Battery() = default;
};

#include "LinearBattery.h"
#include "LithiumBattery.h"

#endif
