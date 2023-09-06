#include "components/SignalGenerator.h"

static float normalizedSquareWave(uint32_t timeMs, uint32_t periodMs, float dutyCycle);

static float normalizedTriangularWave(uint32_t timeMs, uint32_t periodMs);

static float normalizedSinusoidalWave(uint32_t timeMs, uint32_t periodMs);

SignalGenerator::SignalGenerator(TaskManager &taskManager,
                                        Potentiometer &potentiometer,
                                        uint32_t timeResolutionMs,
                                        const char *name,
                                        LogLevel logLevel) :
        Component(name, logLevel),
        Task(timeResolutionMs),
        taskManager(taskManager),
        potentiometer(potentiometer) {}

void SignalGenerator::squareWave(float _levelMin, float _levelMax, uint32_t _periodMs, float _dutyCycle) {
    this->levelMin = _levelMin;
    this->levelMax = _levelMax;
    this->periodMs = _periodMs;
    this->dutyCycle = _dutyCycle;
    startTask(SignalType::SQUARE_WAVE);
}

void SignalGenerator::triangularWave(float _levelMin, float _levelMax, uint32_t _periodMs) {
    this->levelMin = _levelMin;
    this->levelMax = _levelMax;
    this->periodMs = _periodMs;
    this->dutyCycle = 0.5f; // unused. Just set to 0.5 for logging
    startTask(SignalType::TRIANGULAR_WAVE);
}

void SignalGenerator::sinusoidalWave(float _levelMin, float _levelMax, uint32_t _periodMs) {
    this->levelMin = _levelMin;
    this->levelMax = _levelMax;
    this->periodMs = _periodMs;
    this->dutyCycle = 0.5f; // unused. Just set to 0.5 for logging
    startTask(SignalType::SINUSOIDAL_WAVE);
}

void SignalGenerator::startTask(SignalType _signalType) {
    const char *signalName = _signalType == SignalType::SQUARE_WAVE
                             ? "square"
                             : (_signalType == SignalType::TRIANGULAR_WAVE
                                ? "triangular"
                                : "sinusoidal");

    signalType = _signalType;
    logger.info("starting %s wave (min: %f max: %f period: %u dutyCycle: %f)", signalName, levelMin, levelMax, periodMs,
                dutyCycle);
    potentiometer.setLevel(levelMin);
    taskManager.ResetTask(this);
    setRemainingTime(0);
    t_0 = GetTaskTime();
}

void SignalGenerator::stop() {
    logger.info("stopping");
    taskManager.StopTask(this);
}

void SignalGenerator::OnUpdate(uint32_t deltaTime) {
    uint32_t timeMs = TaskTimeToMs(GetTaskTime()) - t_0;
    float normalizedLevel;

    switch (signalType) {
        case SignalType::SINUSOIDAL_WAVE:
            normalizedLevel = normalizedSinusoidalWave(timeMs, periodMs);
            break;
        case SignalType::TRIANGULAR_WAVE:
            normalizedLevel = normalizedTriangularWave(timeMs, periodMs);
            break;
        default:
            normalizedLevel = normalizedSquareWave(timeMs, periodMs, dutyCycle);
    }

    potentiometer.setLevel(levelMin + (levelMax - levelMin) * normalizedLevel);
}

float normalizedSquareWave(uint32_t timeMs, uint32_t periodMs, float dutyCycle) {
    uint32_t remainingTimeMs = timeMs % periodMs;
    return remainingTimeMs < periodMs * dutyCycle ? 0 : 1;
}

float normalizedTriangularWave(uint32_t timeMs, uint32_t periodMs) {
    uint32_t remainingTimeMs = timeMs % periodMs;
    return remainingTimeMs < periodMs / 2
           ? static_cast<float>(remainingTimeMs) / (periodMs / 2)
           : static_cast<float>(-(remainingTimeMs - periodMs)) / (periodMs / 2);
}

float normalizedSinusoidalWave(uint32_t timeMs, uint32_t periodMs) {
    return (-_cos(timeMs * (COMPONENTS_TWO_PI / periodMs)) + 1.0f) / 2.0f;
}
