#ifndef CAPPABLEPOTENTIOMETER_H
#define CAPPABLEPOTENTIOMETER_H

#include "Potentiometer.h"

class CappablePotentiometer: public Potentiometer {
public:
	CappablePotentiometer(const char *name = nullptr);
	void setLevel(float level) override;
	float getLevelMaxLimit();
	void setLevelMaxLimit(float limit);
protected:
	float levelMaxLimit = 1.0f;
	float wantedLevel = 0.0f;
};

inline CappablePotentiometer::CappablePotentiometer(const char *name) :
		Potentiometer(name) {

}

inline float CappablePotentiometer::getLevelMaxLimit() {
	return levelMaxLimit;
}

inline void CappablePotentiometer::setLevel(float level) {
	wantedLevel = level;
	Potentiometer::setLevel(_min(wantedLevel, levelMaxLimit));
}

inline void CappablePotentiometer::setLevelMaxLimit(float limit) {
	logger.trace("max level: %f", limit);

	levelMaxLimit = _constrain(limit, 0.0, 1.0);

	if (levelMaxLimit < level || (level < wantedLevel && levelMaxLimit > level)) {

		/*
		 * we have to reduce the level || we can increase level
		 */
		setLevel(wantedLevel);
	}
}

#endif
