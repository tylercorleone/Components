#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include "Components.h"

enum class SignalType {
	SINUSOIDAL_WAVE, TRIANGULAR_WAVE, SQUARE_WAVE
};

class SignalGenerator {
public:
	SignalGenerator(TaskManager &taskManager, Potentiometer &potentiometer,
			uint32_t timeResolutionMs = MsToTaskTime(10));
	void start(SignalType signalType, float levelMin, float levelMax,
			uint32_t periodMs);
	void squareWave(float levelMin, float levelMax, uint32_t periodMs,
			float dutyCycle);
	void stop();
private:
	static uint32_t updateLevel(SignalGenerator *_this);
	static float sinWave(uint32_t millis, uint32_t periodMs);
	static float triangularWave(uint32_t millis, uint32_t periodMs);

	TaskManager &taskManager;
	Potentiometer &potentiometer;
	SignalType signalType = SignalType::SINUSOIDAL_WAVE;
	Procedure &levelSetter = ProcedureBuilder::begin(
			SignalGenerator::updateLevel, this).thenRepeat();
	uint32_t timeResolutionMs;
	uint32_t t_0 = -1;
	uint32_t periodMs = -1;
	float dutyCycle = 0.0f;
	float levelMin = 0.0f;
	float levelMax = 1.0f;
	// used by square wave
	bool onLevelMin = true;
};

inline SignalGenerator::SignalGenerator(TaskManager &taskManager,
		Potentiometer &potentiometer, uint32_t timeResolutionMs) :
		taskManager(taskManager), potentiometer(potentiometer), timeResolutionMs(
				timeResolutionMs) {

}

inline void SignalGenerator::start(SignalType _signalType,
		float _levelMin, float _levelMax, uint32_t _periodMs) {
	taskManager.StopTask(&levelSetter);
	signalType = _signalType;
	levelMin = _levelMin;
	levelMax = _levelMax;
	periodMs = _periodMs;
	dutyCycle = 0.5f;
	potentiometer.setLevel(levelMin);
	onLevelMin = true;
	taskManager.StartTask(&levelSetter);
	t_0 = MILLIS_PROVIDER();
}

inline void SignalGenerator::squareWave(float _levelMin, float _levelMax, uint32_t _periodMs, float _dutyCycle) {
	start(SignalType::SQUARE_WAVE, _levelMin, _levelMax, _periodMs);
	dutyCycle = _dutyCycle;
}

inline void SignalGenerator::stop() {
	taskManager.StopTask(&levelSetter);
}

inline uint32_t SignalGenerator::updateLevel(SignalGenerator *_this) {
	uint32_t nextIntervalMs;
	float nextLevel;
	switch (_this->signalType) {
	case SignalType::SINUSOIDAL_WAVE:
		nextIntervalMs = _this->timeResolutionMs;
		nextLevel = _this->levelMin
				+ (_this->levelMax - _this->levelMin)
						* (sinWave(MILLIS_PROVIDER() - _this->t_0,
								_this->periodMs));
		break;
	case SignalType::TRIANGULAR_WAVE:
		nextIntervalMs = _this->timeResolutionMs;
		nextLevel = _this->levelMin
				+ (_this->levelMax - _this->levelMin)
						* (triangularWave(MILLIS_PROVIDER() - _this->t_0,
								_this->periodMs));
		break;
	default:
		if (_this->onLevelMin) {
			nextLevel = _this->levelMax;
			nextIntervalMs = _this->periodMs * _this->dutyCycle;
		} else {
			nextLevel = _this->levelMin;
			nextIntervalMs = _this->periodMs * (1.0f - _this->dutyCycle);
		}
		_this->onLevelMin = !_this->onLevelMin;
	}
	_this->potentiometer.setLevel(nextLevel);
	return MsToTaskTime(nextIntervalMs);
}

inline float SignalGenerator::triangularWave(uint32_t millis,
		uint32_t periodMs) {
	millis %= periodMs;
	if (millis < periodMs / 2) {
		return static_cast<float>(millis) / (periodMs / 2);
	} else {
		return static_cast<float>(-(millis - periodMs)) / (periodMs / 2);
	}
}

/**
 * Signal going from 0 to 1, starting from 0 when millis = 0
 */
inline float SignalGenerator::sinWave(uint32_t millis, uint32_t periodMs) {
	return (-_cos(millis * (_TWO_PI / periodMs)) + 1.0f) / 2.0f;
}

#endif
