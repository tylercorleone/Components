#include "components/LightnessDriver.h"

LightnessDriver::LightnessDriver(uint32_t timeInterval,
                                 TaskManager &taskManager,
                                 Potentiometer &brightnessDriver,
                                 const char *name,
                                 LogLevel logLevel) :
        TimedPotentiometer(timeInterval, taskManager, name, logLevel),
        brightnessDriver(brightnessDriver) {}

void LightnessDriver::onSetLevel(float level) {
    float brightnessLevel = lightnessSimulationEnabled ? LIGHTNESS_TO_BRIGHTNESS_CONVERSION(level) : level;
    brightnessDriver.setLevel(brightnessLevel);
}

bool LightnessDriver::isLightnessSimulationEnabled() {
    return lightnessSimulationEnabled;
}

void LightnessDriver::isLightnessSimulationEnabled(bool isEnabled) {
    lightnessSimulationEnabled = isEnabled;
}

void LightnessDriver::onSwitchOn() {
    Potentiometer::onSwitchOn();
    brightnessDriver.setState(OnOffState::ON);
}

void LightnessDriver::onSwitchOff() {
    Potentiometer::onSwitchOff(); // this is important in order to set lightness to 0
    brightnessDriver.setState(OnOffState::OFF);
}

/**
 * @see https://en.wikipedia.org/wiki/CIELAB_color_space
 */
float cieLabConversion(float lightness) {
    if (lightness > 0.08f) {
        float x = (lightness + 0.16f) / 1.16f;
        return components_cube(x);
    } else {
        return 0.11071f * lightness;
    }
}

/**
 * @see https://en.wikipedia.org/wiki/CIELAB_color_space#Hunter_Lab
 */
float hunterLabConversion(float lightness) {
    return lightness * lightness;
}
