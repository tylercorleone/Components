#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Component.h"
#include "commons/Enums.h"
#include "commons/components_utilities.h"

class Potentiometer : public Component {
public:
    Potentiometer(const char *name = "potentiometer",
                  LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);

    float getLevel();

    virtual void setLevel(float level);

    OnOffState getState();

    void setState(OnOffState state);

    void toggleState();

    virtual ~Potentiometer() = default;

protected:
    virtual void onSetLevel(float level) = 0;

    virtual void onSwitchOn();

    virtual void onSwitchOff();

    OnOffState currentState = OnOffState::OFF;
    float currentLevel = 0.0f;
};

#endif
