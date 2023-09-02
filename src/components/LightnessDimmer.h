#ifndef LIGHTNESS_DIMMER_H
#define LIGHTNESS_DIMMER_H

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

class LightnessDimmer : public CappablePotentiometer {
public:

    LightnessDimmer(Potentiometer &brightnessPotentiometer,
                    const char *name = nullptr,
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

inline void LightnessDimmer::onSetLevel(float level) {
    float brightnessPotLevel = lightnessSimulationEnabled ? LIGHTNESS_TO_BRIGHTNESS_CONVERSION(level) : level;
    brightnessPotentiometer.setLevel(brightnessPotLevel);
}

inline bool LightnessDimmer::isLightnessSimulationEnabled() {
    return lightnessSimulationEnabled;
}

inline void LightnessDimmer::isLightnessSimulationEnabled(bool isEnabled) {
    lightnessSimulationEnabled = isEnabled;
}

inline void LightnessDimmer::onSwitchOn() {
    brightnessPotentiometer.setState(OnOffState::ON);
}

inline void LightnessDimmer::onSwitchOff() {
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
