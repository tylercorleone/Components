#ifndef LITHIUM_BATTERY_H
#define LITHIUM_BATTERY_H

#include "Battery.h"

/**
 * Lithium batteries have a voltage/capacity function that
 * can be approximated such a composition of two linear strokes
 *
 * e.g. https://lygte-info.dk/review/batteries2012/Panasonic%20NCR18650B%203400mAh%20(Green)%20UK.html
 */
class LithiumBattery : public Battery {
public:
    LithiumBattery(float (*readVoltageFunc)(void),
                            float voltageEmpty = 3.0f, // 0%
                            float voltageLow = 3.2, // where starts the linear zone through voltage full
                            float voltageFull = 4.2, // 100%
                            float voltageLowCapacity = 0.05, // 0.005 = 5% at 3.2 V
                            const char *name = "lithiumBattery",
                            LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    float getRemainingCharge() const override;

protected:
    float (*readVoltage)(void);

    float voltageEmpty;
    float voltageLow;
    float voltageFull;
    float voltageLowCapacity;
};

inline LithiumBattery::LithiumBattery(float (*readVoltageFunc)(void),
                                      float voltageEmpty,
                                      float voltageLow,
                                      float voltageFull,
                                      float voltageLowCapacity,
                                      const char *name,
                                      LogLevel logLevel) :
        Battery(name, logLevel),
        readVoltage(readVoltageFunc),
        voltageEmpty(voltageEmpty),
        voltageLow(voltageLow),
        voltageFull(voltageFull),
        voltageLowCapacity(voltageLowCapacity) {}

/**
 * An approximation of the charge/voltage function for Lithium batteries
 */
inline float LithiumBattery::getRemainingCharge() const {
    float currentVoltage = readVoltage();
    currentVoltage = _constrain(currentVoltage, voltageEmpty, voltageFull);

    float remainingChange;
    if (currentVoltage < voltageLow) {

        /*
         * low charge zone
         */
        remainingChange = (currentVoltage - voltageEmpty) / (voltageLow - voltageEmpty) * voltageLowCapacity;
    } else {

        /*
         * high charge zone
         */
        remainingChange = voltageLowCapacity
                          + (currentVoltage - voltageLow)
                            / (voltageFull - voltageLow)
                            * (1.0f - voltageLowCapacity);
    }

    return _constrain(remainingChange, 0.0f, 1.0f);
}

#endif
