#include "components/RangeLimitedPotentiometer.h"

RangeLimitedPotentiometer::RangeLimitedPotentiometer(const char *name,
                                                     LogLevel logLevel) :
        Potentiometer(name, logLevel) {}

void RangeLimitedPotentiometer::setLevel(float level) {
    wantedLevel = level;

    float newLevel = components_constrain(level, minValue, maxValue);

    if (newLevel != wantedLevel) {
        logger.debug("limiting level %f to %f", wantedLevel, newLevel);
    }

    Potentiometer::setLevel(newLevel);
}

float RangeLimitedPotentiometer::getMaxValue() {
    return maxValue;
}

void RangeLimitedPotentiometer::setMaxValue(float value) {
    float newValue = components_constrain(value, minValue, 1.0f);

    if (newValue == maxValue) return;

    maxValue = newValue;

    logger.debug("max level set to %f", maxValue);

    if (currentLevel > maxValue || (currentLevel < wantedLevel && currentLevel < maxValue)) {

        /*
         * level is higher than max permitted || level is lower than expected && we have room to increase it
         */
        setLevel(wantedLevel);
    }
}

float RangeLimitedPotentiometer::getMinValue() {
    return minValue;
}

void RangeLimitedPotentiometer::setMinValue(float value) {
    float newValue = components_constrain(value, 0.0f, maxValue);

    if (newValue == minValue) return;

    minValue = newValue;

    logger.debug("min level set to %f", minValue);

    if (currentLevel < minValue || (currentLevel > wantedLevel && currentLevel > minValue)) {

        /*
         * level is lower than min permitted || level is higher than expected && we have room to decrease it
         */
        setLevel(wantedLevel);
    }
}
