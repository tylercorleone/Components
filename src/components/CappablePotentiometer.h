#ifndef CAPPABLEPOTENTIOMETER_H
#define CAPPABLEPOTENTIOMETER_H

#include "Potentiometer.h"

class CappablePotentiometer: public Potentiometer {
public:
	void setLevel(float level) override;
	float getLevelMaxLimit();
	void setLevelMaxLimit(float limit);
	virtual ~CappablePotentiometer();
protected:
	float levelMaxLimit = 1.0f;
	float wantedLevel = -1.0f;
};

inline float CappablePotentiometer::getLevelMaxLimit() {
	return levelMaxLimit;
}

inline void CappablePotentiometer::setLevel(float level) {
	wantedLevel = level;
	Potentiometer::setLevel(min(wantedLevel, levelMaxLimit));
}

inline void CappablePotentiometer::setLevelMaxLimit(float limit) {
	traceIfNamed("setLevelMaxLimit(%f)", limit);

	levelMaxLimit = _constrain(limit, 0.0, 1.0);

	if (levelMaxLimit < level || (level < wantedLevel && levelMaxLimit > level)) {

		/*
		 * we have to reduce the level || we can increase level
		 */
		setLevel(wantedLevel);
	}
}

inline CappablePotentiometer::~CappablePotentiometer() {

}

#endif
