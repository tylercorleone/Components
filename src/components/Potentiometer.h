#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "commons/components_commons.h"

class Potentiometer: public Named {
public:
	float getLevel();
	virtual void setLevel(float level);
	OnOffState getState();
	void setState(OnOffState state);
	void toggleState();
	virtual ~Potentiometer();
protected:
	friend class LightDimmer;
	virtual void onSetLevel(float level) = 0;
	virtual void onSwitchOn();
	virtual void onSwitchOff();

	OnOffState state = OnOffState::OFF;
	float level = -1.0f;
};

inline float Potentiometer::getLevel() {
	return level;
}

inline void Potentiometer::setLevel(float level) {
	traceIfNamed("setLevel(%f)", level);

	this->level = _constrain(level, 0.0f, 1.0f);
	if (state == OnOffState::ON) {
		onSetLevel(this->level);
	}
}

inline OnOffState Potentiometer::getState() {
	return state;
}

inline void Potentiometer::setState(OnOffState state) {
	debugIfNamed("setState(%s)", state == OnOffState::ON ? "ON" : "OFF");

	this->state = state;
	state == OnOffState::ON ? onSwitchOn() : onSwitchOff();
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
