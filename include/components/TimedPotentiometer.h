#ifndef TIMED_POTENTIOMETER_H
#define TIMED_POTENTIOMETER_H

#include "RangeLimitedPotentiometer.h"
#include "task_includer.h"

/**
 * Utility class to set potentiometers' level, min value and max value.
 */
class LevelWorker : protected Task {
public:
    LevelWorker(float (*readValueFunc)(RangeLimitedPotentiometer &),
                void (*writeValueFunc)(RangeLimitedPotentiometer &, float),
                RangeLimitedPotentiometer &potentiometer,
                uint32_t timeInterval,
                TaskManager &taskManager,
                Logger &logger,
                const char *valueName);

    void setValueGradually(float value, uint32_t transitionDurationMs);

protected:
    void OnUpdate(uint32_t timeInterval) override;

    float (*readValue)(RangeLimitedPotentiometer &);

    void (*writeValue)(RangeLimitedPotentiometer &, float value);

    RangeLimitedPotentiometer &potentiometer;
    TaskManager &taskManager;
    Logger &logger;
    const char *valueName;
    float targetLevel = 0.0;
    uint32_t stepsToGo = 0;
};

class TimedPotentiometer : public RangeLimitedPotentiometer {
public:
    TimedPotentiometer(uint32_t timeInterval,
                       TaskManager &taskManager,
                       const char *name = "timedPotentiometer",
                       LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL
    );

    void setLevelGradually(float level, uint32_t duration);

    void setMinValueGradually(float level, uint32_t duration);

    void setMaxValueGradually(float level, uint32_t duration);

protected:
    LevelWorker levelWorker;
    LevelWorker minValueWorker;
    LevelWorker maxValueWorker;

    static float getPotLevel(RangeLimitedPotentiometer &pot);

    static void setPotLevel(RangeLimitedPotentiometer &pot, float level);

    static float getPotMinValue(RangeLimitedPotentiometer &pot);

    static void setPotMinValue(RangeLimitedPotentiometer &pot, float value);

    static float getPotMaxValue(RangeLimitedPotentiometer &pot);

    static void setPotMaxValue(RangeLimitedPotentiometer &pot, float value);
};

#endif
