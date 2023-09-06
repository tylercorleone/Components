#ifndef RANGE_LIMITED_POTENTIOMETER_H
#define RANGE_LIMITED_POTENTIOMETER_H

#include "Potentiometer.h"

class RangeLimitedPotentiometer : public Potentiometer {
public:
    explicit RangeLimitedPotentiometer(const char *name = "rngLimPot",
                                       LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void setLevel(float level) override;

    float getMinValue();

    void setMinValue(float value);

    float getMaxValue();

    void setMaxValue(float value);

protected:
    float maxValue = 1.0f;
    float minValue = 0.0f;
    float wantedLevel = 0.0f;
};

#endif
