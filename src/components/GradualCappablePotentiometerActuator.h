#ifndef GRADUAL_CAPPABLE_POTENTIOMETER_ACTUATOR_H
#define GRADUAL_CAPPABLE_POTENTIOMETER_ACTUATOR_H

#include "GradualPotentiometerActuator.h"
#include "CappablePotentiometer.h"

class GradualLevelMaxValueSetter : public GradualLevelSetter {
public:
    GradualLevelMaxValueSetter(uint32_t timeInterval,
                               TaskManager &taskManager,
                               CappablePotentiometer &cappablePotentiometer,
                               const char *name = nullptr,
                               LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

private:
    float readLevel() override;

    void writeLevel(float level) override;

    CappablePotentiometer &cappablePotentiometer;
};

inline GradualLevelMaxValueSetter::GradualLevelMaxValueSetter(uint32_t timeInterval,
                                                              TaskManager &taskManager,
                                                              CappablePotentiometer &cappablePotentiometer,
                                                              const char *name,
                                                              LogLevel logLevel) :
        GradualLevelSetter(timeInterval, taskManager, name, logLevel),
        cappablePotentiometer(cappablePotentiometer) {
}

inline float GradualLevelMaxValueSetter::readLevel() {
    return cappablePotentiometer.getLevelMaxValue();
}

inline void GradualLevelMaxValueSetter::writeLevel(float level) {
    if (level != cappablePotentiometer.getLevelMaxValue()) {
        cappablePotentiometer.updateLevelMaxValue(level);
    }
}

/**
 *
 */
class GradualCappablePotentiometerActuator : public GradualPotentiometerActuator {
public:
    GradualCappablePotentiometerActuator(uint32_t timeInterval,
                                         TaskManager &taskManager,
                                         CappablePotentiometer &cappablePotentiometer,
                                         const char *name = nullptr,
                                         LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void updateLevelMaxValue(float level, uint32_t duration);

private:
    GradualLevelMaxValueSetter gradualLevelMaxValueSetter{_timeInterval,
                                                          taskManager, (CappablePotentiometer &) potentiometer};
};

inline GradualCappablePotentiometerActuator::GradualCappablePotentiometerActuator(
        uint32_t timeInterval,
        TaskManager &taskManager,
        CappablePotentiometer &cappablePotentiometer,
        const char *name,
        LogLevel logLevel) :
        GradualPotentiometerActuator(timeInterval, taskManager, cappablePotentiometer, name, logLevel) {}

inline void GradualCappablePotentiometerActuator::updateLevelMaxValue(float level,
                                                                      uint32_t duration) {
    logger.debug("setting max to %f in %u ms", level, duration);

    gradualLevelMaxValueSetter.setLevel(level, duration);
}

#endif
