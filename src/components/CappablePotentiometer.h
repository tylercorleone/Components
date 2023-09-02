#ifndef CAPPABLEPOTENTIOMETER_H
#define CAPPABLEPOTENTIOMETER_H

#include "Potentiometer.h"

class CappablePotentiometer : public Potentiometer {
public:
    CappablePotentiometer(const char *name = nullptr,
                          LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Potentiometer(name, logLevel) {}

    void setLevel(float level) override;

    float getMinValue();

    void setMinValue(float limit);

    float getMaxValue();

    void setMaxValue(float limit);

protected:
    float maxValue = 1.0f;
    float minValue = 0.0f;
    float wantedLevel = 0.0f;
};

inline void CappablePotentiometer::setLevel(float level) {
    wantedLevel = level;

    float newLevel = _constrain(level, minValue, maxValue);

    if (newLevel != wantedLevel) {
        logger.debug("limiting level %f to %f", wantedLevel, newLevel);
    }

    Potentiometer::setLevel(newLevel);
}

inline float CappablePotentiometer::getMaxValue() {
    return maxValue;
}

inline void CappablePotentiometer::setMaxValue(float value) {
    float newValue = _constrain(value, minValue, 1.0f);

    if (newValue == maxValue) return;

    maxValue = newValue;

    logger.debug("new max value %f", maxValue);

    if (currentLevel > maxValue || (currentLevel < wantedLevel && currentLevel < maxValue)) {

        /*
         * level is higher than max permitted || level is lower than expected && we have room to increase it
         */
        setLevel(wantedLevel);
    }
}

inline float CappablePotentiometer::getMinValue() {
    return minValue;
}

inline void CappablePotentiometer::setMinValue(float value) {
    float newValue = _constrain(value, 0.0f, maxValue);

    if (newValue == minValue) return;

    minValue = newValue;

    logger.debug("min level updated to %f", minValue);

    if (currentLevel < minValue || (currentLevel > wantedLevel && currentLevel > minValue)) {

        /*
         * level is lower than min permitted || level is higher than expected && we have room to decrease it
         */
        setLevel(wantedLevel);
    }
}

#endif
