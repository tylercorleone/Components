#include "components/batteries/LinearBattery.h"
#include "commons/components_utilities.h"

LinearBattery::LinearBattery(float (*readVoltageFunc)(),
                                    float voltageEmpty,
                                    float voltageFull) :
        readVoltage(readVoltageFunc),
        voltageEmpty(voltageEmpty),
        voltageFull(voltageFull) {}

float LinearBattery::getRemainingCharge() const {
    float _charge = (readVoltage() - voltageEmpty) / (voltageFull - voltageEmpty);
    return components_constrain(_charge, 0.0f, 1.0f);
}
