#ifndef LINEAR_BATTERY_H
#define LINEAR_BATTERY_H

#include "Battery.h"

class LinearBattery : public Battery {
public:
    LinearBattery(float (*readVoltageFunc)(), float voltageEmpty, float voltageFull);

    float getRemainingCharge() const override;

protected:
    float (*readVoltage)();

    float voltageEmpty;
    float voltageFull;
};

#endif
