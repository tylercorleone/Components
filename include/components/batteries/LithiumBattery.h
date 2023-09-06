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
    LithiumBattery(float (*readVoltageFunc)(),
                   float voltageEmpty = 3.0f, // 0%
                   float voltageLow = 3.2, // where starts the linear zone through voltage full
                   float voltageFull = 4.2, // 100%
                   float voltageLowCapacity = 0.05, // 0.005 = 5% at 3.2 V
                   const char *name = "lithiumBattery",
                   LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    float getRemainingCharge() const override;

protected:
    float (*readVoltage)();

    float voltageEmpty;
    float voltageLow;
    float voltageFull;
    float voltageLowCapacity;
};

#endif
