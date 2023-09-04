#ifndef LIGHTNESS_DRIVER_H
#define LIGHTNESS_DRIVER_H

#include "Potentiometer.h"

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

class LightnessDriver : public CappablePotentiometer {
public:

    LightnessDriver(Potentiometer &brightnessPotentiometer,
                    const char *name = "lightnessDriver",
                    LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            CappablePotentiometer(name, logLevel),
            brightnessPotentiometer(brightnessPotentiometer) {}

    bool isLightnessSimulationEnabled();

    void isLightnessSimulationEnabled(bool);

protected:
    void onSetLevel(float level) override;

    void onSwitchOn() override;

    void onSwitchOff() override;

    Potentiometer &brightnessPotentiometer;
    bool lightnessSimulationEnabled = true;
};

inline void LightnessDriver::onSetLevel(float level) {
    float brightnessLevel = lightnessSimulationEnabled ? LIGHTNESS_TO_BRIGHTNESS_CONVERSION(level) : level;
    brightnessPotentiometer.setLevel(brightnessLevel);
}

inline bool LightnessDriver::isLightnessSimulationEnabled() {
    return lightnessSimulationEnabled;
}

inline void LightnessDriver::isLightnessSimulationEnabled(bool isEnabled) {
    lightnessSimulationEnabled = isEnabled;
}

inline void LightnessDriver::onSwitchOn() {
    Potentiometer::onSwitchOn();
    brightnessPotentiometer.setState(OnOffState::ON);
}

inline void LightnessDriver::onSwitchOff() {
    Potentiometer::onSwitchOff(); // important to set lightness to 0
    brightnessPotentiometer.setState(OnOffState::OFF);
}

/**
 * @see https://en.wikipedia.org/wiki/CIELAB_color_space
 */
inline float cieLabConversion(float lightness) {
    if (lightness > 0.08f) {
        float x = (lightness + 0.16f) / 1.16f;
        return _cube(x);
    } else {
        return 0.11071f * lightness;
    }
}

/**
 * @see https://en.wikipedia.org/wiki/CIELAB_color_space#Hunter_Lab
 */
inline float hunterLabConversion(float lightness) {
    return lightness * lightness;
}

#endif
