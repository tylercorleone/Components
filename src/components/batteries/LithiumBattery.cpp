#include "components/batteries/LithiumBattery.h"
#include "commons/components_utilities.h"

LithiumBattery::LithiumBattery(float (*readVoltageFunc)(void),
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
float LithiumBattery::getRemainingCharge() const {
    float currentVoltage = readVoltage();
    currentVoltage = components_constrain(currentVoltage, voltageEmpty, voltageFull);

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

    return components_constrain(remainingChange, 0.0f, 1.0f);
}
