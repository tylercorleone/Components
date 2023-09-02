#ifndef BATTERY_H
#define BATTERY_H

#include "Components.h"

/**
 * Battery is a component that can be inspected
 * to get the remaining charge level
 */
class Battery : public Component {
public:
    Battery(const char *name = nullptr,
            LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Component(name, logLevel) {}

    virtual float getRemainingCharge() const = 0;

    virtual ~Battery() {}
};

class GenericBattery : public Battery {
public:
    GenericBattery(float (*readRemainingChargeFunc)(),
                   const char *name = nullptr,
                   LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL)
            : Battery(name, logLevel),
              readRemainingCharge(readRemainingChargeFunc) {}

    float getRemainingCharge() const override;

protected:
    float (*readRemainingCharge)(void);
};

class LinearCapacityBattery : public Battery {
public:
    LinearCapacityBattery(float voltageEmpty, float voltageFull,
                          float (*readVoltageFunc)());

    float getRemainingCharge() const override;

protected:
    float voltageEmpty;
    float voltageFull;

    float (*readVoltage)(void);
};

inline float GenericBattery::getRemainingCharge() const {
    return readRemainingCharge();
}

inline LinearCapacityBattery::LinearCapacityBattery(float voltageEmpty,
                                                    float voltageFull, float (*readVoltageFunc)()) :
        voltageEmpty(voltageEmpty), voltageFull(voltageFull), readVoltage(
        readVoltageFunc) {

}

inline float LinearCapacityBattery::getRemainingCharge() const {
    float _charge = (readVoltage() - voltageEmpty) / (voltageFull - voltageEmpty);
    return _constrain(_charge, 0.0f, 1.0f);
}

#endif
