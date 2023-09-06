#ifndef LIGHTNESS_DRIVER_H
#define LIGHTNESS_DRIVER_H

#include "TimedPotentiometer.h"

float cieLabConversion(float);

float hunterLabConversion(float);

/**
 * Lightness to brightness conversion

 * lightness = the human-perceived brightness
 * brightness = measure of the luminous intensity
 */
#ifndef LIGHTNESS_TO_BRIGHTNESS_CONVERSION
#define LIGHTNESS_TO_BRIGHTNESS_CONVERSION(lightness) cieLabConversion(lightness)
#endif

class LightnessDriver : public TimedPotentiometer {
public:

    LightnessDriver(uint32_t timeInterval,
                    TaskManager &taskManager,
                    Potentiometer &brightnessDriver,
                    const char *name = "lightnessDriver",
                    LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    bool isLightnessSimulationEnabled();

    void isLightnessSimulationEnabled(bool);

protected:
    void onSetLevel(float level) override;

    void onSwitchOn() override;

    void onSwitchOff() override;

    Potentiometer &brightnessDriver;
    bool lightnessSimulationEnabled = true;
};

#endif
