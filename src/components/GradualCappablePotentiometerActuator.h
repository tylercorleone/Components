#ifndef GRADUAL_CAPPABLE_POTENTIOMETER_ACTUATOR_H
#define GRADUAL_CAPPABLE_POTENTIOMETER_ACTUATOR_H

#include "GradualPotentiometerActuator.h"
#include "CappablePotentiometer.h"

class GradualLevelMaxValueSetter : public GradualLevelSetter {
public:
    GradualLevelMaxValueSetter(uint32_t timeInterval,
                               TaskManager &taskManager,
                               CappablePotentiometer &cappablePotentiometer,
                               const char *name = "gradLevelMaxValueSetter",
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
    return cappablePotentiometer.getMaxValue();
}

inline void GradualLevelMaxValueSetter::writeLevel(float level) {
    cappablePotentiometer.setMaxValue(level);
}

/**
 *
 */
class GradualCappablePotentiometerActuator : public GradualPotentiometerActuator {
public:
    GradualCappablePotentiometerActuator(uint32_t timeInterval,
                                         TaskManager &taskManager,
                                         CappablePotentiometer &cappablePotentiometer,
                                         const char *name = "gradCappablePotActuator",
                                         LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void setMaxValueGradually(float level, uint32_t duration);

private:
    GradualLevelMaxValueSetter gradualLevelMaxValueSetter;
};

inline GradualCappablePotentiometerActuator::GradualCappablePotentiometerActuator(uint32_t timeInterval,
                                                                                  TaskManager &taskManager,
                                                                                  CappablePotentiometer &cappablePotentiometer,
                                                                                  const char *name,
                                                                                  LogLevel logLevel) :
        GradualPotentiometerActuator(timeInterval, taskManager, cappablePotentiometer, name, logLevel),
        gradualLevelMaxValueSetter(timeInterval, taskManager, cappablePotentiometer, name, logLevel) {}

inline void GradualCappablePotentiometerActuator::setMaxValueGradually(float level,
                                                                      uint32_t duration) {
    logger.debug("setting %s's max to %f in %u ms", potentiometer.getIdentifier(), level, duration);

    gradualLevelMaxValueSetter.setLevelGradually(level, duration);
}

#endif
