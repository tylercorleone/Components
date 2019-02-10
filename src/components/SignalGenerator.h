#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include "Components.h"

enum class SignalType {
	SINUSOIDAL_WAVE, TRIANGULAR_WAVE, SQUARE_WAVE
};

class SignalGenerator: private Task {
public:
	SignalGenerator(TaskManager &taskManager, Potentiometer &potentiometer,
			uint32_t timeResolution = MsToTaskTime(10));
	void start(SignalType signalType, float levelMin, float levelMax,
			uint32_t period);
	void squareWave(float levelMin, float levelMax, uint32_t period,
			float dutyCycle);
	void stop();
private:
	void OnUpdate(uint32_t deltaTime) override;
	static float sinWave(uint32_t taskTime, uint32_t period);
	static float triangularWave(uint32_t taskTime, uint32_t period);

	TaskManager &taskManager;
	Potentiometer &potentiometer;
	SignalType signalType = SignalType::SINUSOIDAL_WAVE;
	uint32_t timeResolution;
	uint32_t t_0 = -1;
	uint32_t period = -1;
	float dutyCycle = 0.0f;
	float levelMin = 0.0f;
	float levelMax = 1.0f;
	// used by square wave
	bool isOnLevelMin = true;
};

inline SignalGenerator::SignalGenerator(TaskManager &taskManager,
		Potentiometer &potentiometer, uint32_t timeResolution) :
		Task(0), taskManager(taskManager), potentiometer(potentiometer), timeResolution(
				timeResolution) {

}

inline void SignalGenerator::start(SignalType _signalType, float _levelMin,
		float _levelMax, uint32_t _period) {
	signalType = _signalType;
	levelMin = _levelMin;
	levelMax = _levelMax;
	period = _period;
	dutyCycle = 0.5f;
	potentiometer.setLevel(levelMin);
	isOnLevelMin = true;
	taskManager.ResetTask(this);
	setRemainingTime(0);
	t_0 = GetTaskTime();
}

inline void SignalGenerator::squareWave(float _levelMin, float _levelMax,
		uint32_t _period, float _dutyCycle) {
	start(SignalType::SQUARE_WAVE, _levelMin, _levelMax, _period);
	dutyCycle = _dutyCycle;
}

inline void SignalGenerator::stop() {
	taskManager.StopTask(this);
}

inline void SignalGenerator::OnUpdate(uint32_t deltaTime) {
	float nextLevel;
	switch (signalType) {
	case SignalType::SINUSOIDAL_WAVE:
		nextLevel = levelMin
				+ (levelMax - levelMin)
						* (sinWave(GetTaskTime() - t_0, period));
		setTimeInterval(timeResolution);
		break;
	case SignalType::TRIANGULAR_WAVE:
		nextLevel = levelMin
				+ (levelMax - levelMin)
						* (triangularWave(GetTaskTime() - t_0, period));
		setTimeInterval(timeResolution);
		break;
	default:
		if (isOnLevelMin) {
			nextLevel = levelMax;
			setTimeInterval(period * dutyCycle);
		} else {
			nextLevel = levelMin;
			setTimeInterval(period * (1.0f - dutyCycle));
		}
		isOnLevelMin = !isOnLevelMin;
	}
	potentiometer.setLevel(nextLevel);
}

inline float SignalGenerator::triangularWave(uint32_t taskTime,
		uint32_t period) {
	taskTime %= period;
	if (taskTime < period / 2) {
		return static_cast<float>(taskTime) / (period / 2);
	} else {
		return static_cast<float>(-(taskTime - period)) / (period / 2);
	}
}

/**
 * Signal going from 0 to 1, starting from 0 when taskTime = 0
 */
inline float SignalGenerator::sinWave(uint32_t taskTime, uint32_t period) {
	return (-_cos(taskTime * (_TWO_PI / period)) + 1.0f) / 2.0f;
}

#endif
