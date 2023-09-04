#ifndef GRADUAL_LEVEL_SETTER_H
#define GRADUAL_LEVEL_SETTER_H

#include "Components.h"

/**
 * Can be programmed to set potentiometers' level, min value, max value etc.
 */
class GradualLevelSetter : public Component, protected Task {
public:
    GradualLevelSetter(uint32_t timeInterval,
                       TaskManager &taskManager,
                       const char *name = "gradLevelSetter",
                       LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Component(name, logLevel),
            Task(timeInterval),
            taskManager(taskManager) {}

    void setLevelGradually(float level, uint32_t transitionDurationMs);

    virtual ~GradualLevelSetter() {}

protected:
    virtual float readLevel() = 0;

    virtual void writeLevel(float level) = 0;

    TaskManager &taskManager;
private:
    void OnUpdate(uint32_t timeInterval) override;

    float targetLevel = 0.0;
    uint32_t stepsToGo = 0;
};

inline void GradualLevelSetter::setLevelGradually(float level, uint32_t duration) {

    taskManager.StopTask(this);

    if (level == readLevel()) {
        return;
    }

    logger.debug("setting level %f in %u ms", level, duration);

    stepsToGo = duration / _timeInterval;

    if (stepsToGo) {
        targetLevel = level;
        taskManager.StartTask(this);
    } else {
        writeLevel(level);
    }
}

inline void GradualLevelSetter::OnUpdate(uint32_t deltaTime) {
    if (stepsToGo == 1) {
        writeLevel(targetLevel);
        taskManager.StopTask(this);
    } else {
        float currentLevel = readLevel();
        writeLevel(currentLevel + (targetLevel - currentLevel) / stepsToGo);
        --stepsToGo;
    }
}

#endif
