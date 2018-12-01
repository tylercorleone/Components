#ifndef LIGHTDIMMER_H
#define LIGHTDIMMER_H

#include "Potentiometer.h"

/**
 * Lightness to brightness conversion

 * lightness = the human-perceived brightness (L* of the CIE L*a*b* color space: https://en.wikipedia.org/wiki/CIELAB_color_space)
 * brightness = measure of the luminous intensity (Y of the CIE 1931 XYZ color space: https://en.wikipedia.org/wiki/CIE_1931_color_space)
 */
#define _cube(x) ((x)*(x)*(x))
#define lightnessToBrightness(lightness) ((lightness) > 0.08f ? _cube(((lightness) + 0.16f) / 1.16f) : (0.11071f * (lightness)))

class LightDimmer: public Potentiometer {
public:
	LightDimmer(Potentiometer &brightnessPotentiometer);
	virtual void setLevel(float level) override;
	bool isLightnessSimulationEnabled();
	void isLightnessSimulationEnabled(bool);
	virtual ~LightDimmer();
protected:
	void onSetLevel(float level) override;
	void onSwitchOn() override;
	void onSwitchOff() override;
	Potentiometer &brightnessPotentiometer;
	bool lightnessSimulationEnabled = true;
};

inline LightDimmer::LightDimmer(Potentiometer &brightnessPotentiometer) :
		brightnessPotentiometer(brightnessPotentiometer) {

}

inline void LightDimmer::setLevel(float level) {
	Potentiometer::setLevel(level);
	if (state == OnOffState::OFF) {
		onSetLevel(this->level);
	}
}

inline void LightDimmer::onSetLevel(float level) {
	float brightnessPotLevel = lightnessSimulationEnabled ? lightnessToBrightness(level) : level;
	brightnessPotentiometer.setLevel(brightnessPotLevel);
}

inline bool LightDimmer::isLightnessSimulationEnabled() {
	return lightnessSimulationEnabled;
}

inline void LightDimmer::isLightnessSimulationEnabled(bool isEnabled) {
	lightnessSimulationEnabled = isEnabled;
}

inline void LightDimmer::onSwitchOn() {
	brightnessPotentiometer.setState(OnOffState::ON);
}

inline void LightDimmer::onSwitchOff() {
	brightnessPotentiometer.setState(OnOffState::OFF);
}

inline LightDimmer::~LightDimmer() {

}

#endif
