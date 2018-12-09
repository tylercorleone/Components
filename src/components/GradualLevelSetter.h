#ifndef GRADUALLEVELSETTER_H
#define GRADUALLEVELSETTER_H

#include "Components.h"

#include <stdint.h>
#include <stddef.h>
#include <Task.h>

class GradualLevelSetter: public Task, public Component {
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
	void OnStop() override;
	float targetLevel = 0.0;
	uint32_t stepsToGo = 0;
};

inline GradualLevelSetter::GradualLevelSetter(uint32_t timeInterval,
		TaskManager &taskManager) :
		Task(timeInterval), taskManager(taskManager) {
}

inline void GradualLevelSetter::setLevel(float level, uint32_t duration) {
	if (getTaskState() == TaskState_Running) {
		taskManager.StopTask(this);
	}

	stepsToGo = duration / _timeInterval;

	if (stepsToGo == 0) {
		writeLevel(level);
		OnStop();
	} else if (readLevel() == level) {
		OnStop();
	} else {
		targetLevel = level;
		taskManager.StartTask(this);
	}
}

inline void GradualLevelSetter::OnUpdate(uint32_t deltaTime) {
	if (stepsToGo == 1) {
		writeLevel(targetLevel);
		taskManager.StopTask(this);
	} else {
		float currentLevel = readLevel();
		writeLevel(currentLevel + (targetLevel - currentLevel) / stepsToGo);
	}

	--stepsToGo;
}

inline void GradualLevelSetter::OnStop() {

}

inline GradualLevelSetter::~GradualLevelSetter() {

}

#endif
