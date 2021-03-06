#ifndef GRADUALLEVELSETTER_H
#define GRADUALLEVELSETTER_H

#include "Components.h"

class GradualLevelSetter: protected Task, public Component {
public:
	GradualLevelSetter(uint32_t timeInterval, TaskManager &taskManager);
	void setLevel(float level, uint32_t transitionDurationMs);
	virtual ~GradualLevelSetter();
protected:
	virtual float readLevel() = 0;
	virtual void writeLevel(float level) = 0;
	TaskManager &taskManager;
private:
	void OnUpdate(uint32_t timeInterval) override;
	float targetLevel = 0.0;
	uint32_t stepsToGo = 0;
};

inline GradualLevelSetter::GradualLevelSetter(uint32_t timeInterval,
		TaskManager &taskManager) :
		Task(timeInterval), taskManager(taskManager) {
}

inline void GradualLevelSetter::setLevel(float level, uint32_t duration) {
	logger.debug("setLevel(%f, %u)", level, duration);

	taskManager.StopTask(this);

	stepsToGo = readLevel() == level ? 0 : duration / _timeInterval;

	if (stepsToGo) {
		targetLevel = level;
		taskManager.StartTask(this);
	} else {
		writeLevel(level);
	}
}

inline void GradualLevelSetter::OnUpdate(uint32_t deltaTime) {
	if (stepsToGo == 1) {
		writeLevel(targetLevel);
		taskManager.StopTask(this);
	} else {
		float currentLevel = readLevel();
		writeLevel(currentLevel + (targetLevel - currentLevel) / stepsToGo);
		--stepsToGo;
	}
}

inline GradualLevelSetter::~GradualLevelSetter() {

}

#endif
