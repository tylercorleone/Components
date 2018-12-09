#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Components.h"

class Potentiometer: public Component {
public:
	Potentiometer(const char *name = nullptr);
	float getLevel();
	virtual void setLevel(float level);
	OnOffState getState();
	void setState(OnOffState state);
	void toggleState();
	virtual ~Potentiometer();
protected:
	friend class LightnessDimmer;
	virtual void onSetLevel(float level) = 0;
	virtual void onSwitchOn();
	virtual void onSwitchOff();

	OnOffState state = OnOffState::OFF;
	float level = -1.0f;
};

inline Potentiometer::Potentiometer(const char *name) :
		Component(name) {

}

inline float Potentiometer::getLevel() {
	return level;
}

inline void Potentiometer::setLevel(float level) {
	this->level = _constrain(level, 0.0f, 1.0f);
	if (state == OnOffState::ON) {
		logger.trace("onSetLevel(%f)", this->level);
		onSetLevel(this->level);
	}
}

inline OnOffState Potentiometer::getState() {
	return state;
}

inline void Potentiometer::setState(OnOffState state) {
	logger.debug("onSwitch%s", state == OnOffState::ON ? "On" : "Off");

	this->state = state;

	if (state == OnOffState::ON) {
		onSwitchOn();
	} else {
		onSwitchOff();
	}
}

inline void Potentiometer::toggleState() {
	setState(state == OnOffState::ON ? OnOffState::OFF : OnOffState::ON);
}

inline void Potentiometer::onSwitchOff() {
	onSetLevel(0.0);
}

inline void Potentiometer::onSwitchOn() {
	onSetLevel(this->level);
}

inline Potentiometer::~Potentiometer() {

}

#endif
