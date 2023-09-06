#include "components/TimedPotentiometer.h"

LevelWorker::LevelWorker(float (*readValueFunc)(RangeLimitedPotentiometer &),
                         void (*writeValueFunc)(RangeLimitedPotentiometer &, float),
                         RangeLimitedPotentiometer &potentiometer,
                         uint32_t timeInterval,
                         TaskManager &taskManager,
                         Logger &logger,
                         const char *valueName) :
        Task(timeInterval),
        readValue(readValueFunc),
        writeValue(writeValueFunc),
        potentiometer(potentiometer),
        taskManager(taskManager),
        logger(logger),
        valueName(valueName) {}


void LevelWorker::setValueGradually(float value, uint32_t duration) {

    taskManager.StopTask(this);

    if (value == readValue(potentiometer)) {
        return;
    }

    logger.debug("setting %s to %f in %u ms", valueName, value, duration);

    stepsToGo = duration / _timeInterval;

    if (stepsToGo) {
        targetLevel = value;
        taskManager.StartTask(this);
    } else {
        writeValue(potentiometer, value);
    }
}

void LevelWorker::OnUpdate(uint32_t deltaTime) {
    if (stepsToGo == 1) {
        writeValue(potentiometer, targetLevel);
        taskManager.StopTask(this);
    } else {
        float currentLevel = readValue(potentiometer);
        writeValue(potentiometer, currentLevel + (targetLevel - currentLevel) / stepsToGo);
        --stepsToGo;
    }
}

TimedPotentiometer::TimedPotentiometer(uint32_t timeInterval,
                                       TaskManager &taskManager,
                                       const char *name,
                                       LogLevel logLevel) :
        RangeLimitedPotentiometer(name, logLevel),
        levelWorker(getPotLevel, setPotLevel, *this, timeInterval, taskManager, logger, "level"),
        minValueWorker(getPotMinValue, setPotMinValue, *this, timeInterval, taskManager, logger, "min"),
        maxValueWorker(getPotMaxValue, setPotMaxValue, *this, timeInterval, taskManager, logger, "max") {}

void TimedPotentiometer::setLevelGradually(float level, uint32_t duration) {
    levelWorker.setValueGradually(level, duration);
}

void TimedPotentiometer::setMinValueGradually(float minValue, uint32_t duration) {
    minValueWorker.setValueGradually(minValue, duration);
}

void TimedPotentiometer::setMaxValueGradually(float maxValue, uint32_t duration) {
    maxValueWorker.setValueGradually(maxValue, duration);
}

float TimedPotentiometer::getPotLevel(RangeLimitedPotentiometer &pot) {
    return pot.getLevel();
}

void TimedPotentiometer::setPotLevel(RangeLimitedPotentiometer &pot, float level) {
    pot.setLevel(level);
}

float TimedPotentiometer::getPotMinValue(RangeLimitedPotentiometer &pot) {
    return pot.getMinValue();
}

void TimedPotentiometer::setPotMinValue(RangeLimitedPotentiometer &pot, float value) {
    pot.setMinValue(value);
}

float TimedPotentiometer::getPotMaxValue(RangeLimitedPotentiometer &pot) {
    return pot.getMaxValue();
}

void TimedPotentiometer::setPotMaxValue(RangeLimitedPotentiometer &pot, float value) {
    pot.setMaxValue(value);
}
