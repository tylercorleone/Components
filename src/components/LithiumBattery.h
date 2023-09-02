#ifndef LITHIUMBATTERY_H
#define LITHIUMBATTERY_H

#include "Battery.h"

/**
 * Lithium batteries have a voltage/capacity function that
 * can be approximated such a composition of two linear strokes
 *
 * e.g. https://lygte-info.dk/review/batteries2012/Panasonic%20NCR18650B%203400mAh%20(Green)%20UK.html
 */
class LithiumBattery : public Battery {
public:
    LithiumBattery(float voltageEmpty,
                   float firstLinearStepEndVoltage,
                   float voltageFull,
                   float firstLinearStepEndCapacity,
                   float (*readVoltageFunc)(void),
                   const char *name = nullptr,
                   LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    float getRemainingCharge() const override;

    float getVoltageFull();

    float getVoltageEmpty();

private:
    float (*readVoltage)(void);

    float voltageFull;
    float voltageEmpty;
    float firstLinearStepEndVoltage;
    float firstLinearStepEndCapacity;
};

inline LithiumBattery::LithiumBattery(float voltageEmpty,
                                      float firstLinearStepEndVoltage,
                                      float voltageFull,
                                      float firstLinearStepEndCapacity,
                                      float (*readVoltageFunc)(void),
                                      const char *name,
                                      LogLevel logLevel) :
        Battery(name, logLevel),
        readVoltage(readVoltageFunc),
        voltageFull(voltageFull),
        voltageEmpty(voltageEmpty),
        firstLinearStepEndVoltage(firstLinearStepEndVoltage),
        firstLinearStepEndCapacity(firstLinearStepEndCapacity) {
}

inline float LithiumBattery::getVoltageFull() {
    return voltageFull;
}

inline float LithiumBattery::getVoltageEmpty() {
    return voltageEmpty;
}

/**
 * An approximation of the charge/voltage function for Lithium batteries
 */
inline float LithiumBattery::getRemainingCharge() const {
    float currentVoltage = readVoltage();
    currentVoltage = _constrain(currentVoltage, voltageEmpty, voltageFull);

    if (currentVoltage < firstLinearStepEndCapacity) {

        /*
         * low charge zone
         */
        return (currentVoltage - voltageEmpty) * (firstLinearStepEndCapacity)
               / (firstLinearStepEndVoltage - voltageEmpty);
    } else {

        /*
         * high charge zone
         */
        return firstLinearStepEndCapacity
               + (currentVoltage - firstLinearStepEndVoltage)
                 * (1.0f - firstLinearStepEndCapacity)
                 / (voltageFull - firstLinearStepEndVoltage);
    }
}

#endif
