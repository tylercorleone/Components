#ifndef GRADUALCAPPABLEPOTENTIOMETERACTUATOR_H
#define GRADUALCAPPABLEPOTENTIOMETERACTUATOR_H

#include "GradualPotentiometerActuator.h"
#include "CappablePotentiometer.h"

class GradualLevelMaxLimitSetter: public GradualLevelSetter {
public:
	GradualLevelMaxLimitSetter(uint32_t timeInterval, TaskManager &taskManager,
			CappablePotentiometer &cappablePotentiometer);
private:
	CappablePotentiometer &cappablePotentiometer;
	float readLevel() override;
	void writeLevel(float level) override;
};

class GradualCappablePotentiometerActuator: public GradualPotentiometerActuator {
public:
	GradualCappablePotentiometerActuator(uint32_t timeInterval,
			TaskManager &taskManager,
			CappablePotentiometer &cappablePotentiometer);
	void setLevelMaxLimit(float level, uint32_t duration);
private:
	GradualLevelMaxLimitSetter gradualLevelMaxLimitSetter {_timeInterval,
		taskManager, (CappablePotentiometer&) potentiometer };
};

inline GradualLevelMaxLimitSetter::GradualLevelMaxLimitSetter(
		uint32_t timeInterval, TaskManager &taskManager,
		CappablePotentiometer &cappablePotentiometer) :
		GradualLevelSetter(timeInterval, taskManager), cappablePotentiometer(
				cappablePotentiometer) {
}

inline float GradualLevelMaxLimitSetter::readLevel() {
	return cappablePotentiometer.getLevelMaxLimit();
}

inline void GradualLevelMaxLimitSetter::writeLevel(float level) {
	cappablePotentiometer.setLevelMaxLimit(level);
}

inline GradualCappablePotentiometerActuator::GradualCappablePotentiometerActuator(
		uint32_t timeInterval, TaskManager &taskManager,
		CappablePotentiometer &cappablePotentiometer) :
		GradualPotentiometerActuator(timeInterval, taskManager,
				cappablePotentiometer) {
}

inline void GradualCappablePotentiometerActuator::setLevelMaxLimit(float level,
		uint32_t duration) {
	logger.debug("setLevelMaxLimit(%f, %u)", level, duration);

	gradualLevelMaxLimitSetter.setLevel(level, duration);
}

#endif
