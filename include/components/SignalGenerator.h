#ifndef SIGNAL_GENERATOR_H
#define SIGNAL_GENERATOR_H

#include "Potentiometer.h"
#include "task_includer.h"

enum class SignalType {
    SQUARE_WAVE, TRIANGULAR_WAVE, SINUSOIDAL_WAVE
};

class SignalGenerator : public Component, private Task {
public:
    SignalGenerator(TaskManager &taskManager,
                    Potentiometer &potentiometer,
                    uint32_t timeResolutionMs = 10,
                    const char *name = "signalGenerator",
                    LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    void squareWave(float levelMin, float levelMax, uint32_t period, float dutyCycle = 0.5f);

    void triangularWave(float levelMin, float levelMax, uint32_t period);

    void sinusoidalWave(float levelMin, float levelMax, uint32_t period);

    void stop();

private:
    void startTask(SignalType signalType);

    void OnUpdate(uint32_t deltaTime) override;

    TaskManager &taskManager;
    Potentiometer &potentiometer;
    SignalType signalType = SignalType::SQUARE_WAVE;
    uint32_t t_0 = -1;
    uint32_t periodMs = -1;
    float dutyCycle = 0.5f;
    float levelMin = 0.0f;
    float levelMax = 1.0f;
};

#endif
