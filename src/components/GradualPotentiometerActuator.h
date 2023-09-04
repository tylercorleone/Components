#ifndef GRADUALPOTENTIOMETERACTUATOR_H
#define GRADUALPOTENTIOMETERACTUATOR_H

#include "GradualLevelSetter.h"
#include "Potentiometer.h"

#include <stdint.h>

class TaskManager;

class GradualPotentiometerActuator : public GradualLevelSetter {
public:
    GradualPotentiometerActuator(uint32_t timeInterval,
                                 TaskManager &taskManager,
                                 Potentiometer &potentiometer,
                                 const char *name = "gradPotActuator",
                                 LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            GradualLevelSetter(timeInterval, taskManager, name, logLevel),
            potentiometer(potentiometer) {}

protected:
    float readLevel() override;

    void writeLevel(float level) override;

    Potentiometer &potentiometer;
};

inline float GradualPotentiometerActuator::readLevel() {
    return potentiometer.getLevel();
}

inline void GradualPotentiometerActuator::writeLevel(float level) {
    potentiometer.setLevel(level);
}

#endif
