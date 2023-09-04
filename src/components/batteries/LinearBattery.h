#ifndef LINEAR_BATTERY_H
#define LINEAR_BATTERY_H

#include "Battery.h"

class LinearBattery : public Battery {
public:
    LinearBattery(float (*readVoltageFunc)(), float voltageEmpty, float voltageFull);

    float getRemainingCharge() const override;

protected:
    float (*readVoltage)(void);

    float voltageEmpty;
    float voltageFull;
};

inline LinearBattery::LinearBattery(float (*readVoltageFunc)(),
                                                    float voltageEmpty,
                                                    float voltageFull) :
        readVoltage(readVoltageFunc),
        voltageEmpty(voltageEmpty),
        voltageFull(voltageFull) {}

inline float LinearBattery::getRemainingCharge() const {
    float _charge = (readVoltage() - voltageEmpty) / (voltageFull - voltageEmpty);
    return _constrain(_charge, 0.0f, 1.0f);
}

#endif
