#ifndef BATTERY_H
#define BATTERY_H

#include "../Component.h"

/**
 * Battery is a component that can be inspected
 * to get the remaining charge level
 */
class Battery : public Component {
public:
    Battery(const char *name = "battery",
            LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Component(name, logLevel) {}

    virtual float getRemainingCharge() const = 0;

    virtual ~Battery() {}
};

class LinearCapacityBattery : public Battery {
public:
    LinearCapacityBattery(float (*readVoltageFunc)(), float voltageEmpty, float voltageFull);

    float getRemainingCharge() const override;

protected:
    float (*readVoltage)(void);

    float voltageEmpty;
    float voltageFull;
};

inline LinearCapacityBattery::LinearCapacityBattery(float (*readVoltageFunc)(),
                                                    float voltageEmpty,
                                                    float voltageFull) :
        readVoltage(readVoltageFunc),
        voltageEmpty(voltageEmpty),
        voltageFull(voltageFull) {}

inline float LinearCapacityBattery::getRemainingCharge() const {
    float _charge = (readVoltage() - voltageEmpty) / (voltageFull - voltageEmpty);
    return _constrain(_charge, 0.0f, 1.0f);
}

#include "LinearBattery.h"
#include "LithiumBattery.h"

#endif
