#ifndef CAPPABLEPOTENTIOMETER_H
#define CAPPABLEPOTENTIOMETER_H

#include "Potentiometer.h"

class CappablePotentiometer : public Potentiometer {
public:
    CappablePotentiometer(const char *name = nullptr,
                          LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Potentiometer(name, logLevel) {}

    void setLevel(float level) override;

    float getLevelMaxValue();

    void updateLevelMaxValue(float limit);

    float getLevelMinValue();

    void updateLevelMinValue(float limit);

protected:
    float levelMaxValue = 1.0f;
    float levelMinValue = 0.0f;
    float currentWantedLevel = 0.0f;
};

inline void CappablePotentiometer::setLevel(float wantedLevel) {
    currentWantedLevel = wantedLevel;

    float actuableLevel;
    if (wantedLevel > levelMaxValue) {
        actuableLevel = levelMaxValue;
    } else if (wantedLevel < levelMinValue) {
        actuableLevel = levelMinValue;
    } else {
        actuableLevel = wantedLevel;
    }

    if (actuableLevel != wantedLevel) {
        logger.debug("capping level %f to %f", wantedLevel, actuableLevel);
    }
    
    Potentiometer::setLevel(actuableLevel);
}

inline float CappablePotentiometer::getLevelMaxValue() {
    return levelMaxValue;
}

inline void CappablePotentiometer::updateLevelMaxValue(float value) {
    float newValue = _constrain(value, levelMinValue, 1.0f);

    if (newValue == levelMaxValue) return;

    levelMaxValue = newValue;

    logger.debug("max value updated to %f", levelMaxValue);

    if (currentLevel > levelMaxValue || (currentLevel < currentWantedLevel && currentLevel < levelMaxValue)) {

        /*
         * level is higher than max permitted || level is lower than expected && we have room to increase it
         */
        setLevel(currentWantedLevel);
    }
}

inline float CappablePotentiometer::getLevelMinValue() {
    return levelMinValue;
}

inline void CappablePotentiometer::updateLevelMinValue(float value) {
    float newValue = _constrain(value, 0.0f, levelMaxValue);

    if (newValue == levelMinValue) return;

    levelMinValue = newValue;

    logger.debug("min level updated to %f", levelMinValue);

    if (currentLevel < levelMinValue || (currentLevel > currentWantedLevel && currentLevel > levelMinValue)) {

        /*
         * level is lower than min permitted || level is higher than expected && we have room to decrease it
         */
        setLevel(currentWantedLevel);
    }
}

#endif
