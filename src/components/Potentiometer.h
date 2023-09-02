#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "Components.h"

class Potentiometer : public Component {
public:
    inline Potentiometer(const char *name = nullptr,
                         LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) :
            Component(name, logLevel) {
    }

    float getLevel() { return currentLevel; }

    virtual void setLevel(float level);

    OnOffState getState() { return currentState; }

    void setState(OnOffState state);

    void toggleState();

    virtual ~Potentiometer() {}

protected:
    virtual void onSetLevel(float desiredLevel) = 0;

    virtual void onSwitchOn();

    virtual void onSwitchOff();

    OnOffState currentState = OnOffState::OFF;
    float currentLevel = 0.0f;
};

inline void Potentiometer::setLevel(float level) {
    this->currentLevel = _constrain(level, 0.0f, 1.0f);

    logger.trace("setting level %f%s", this->currentLevel, currentState == OnOffState::OFF ? " while OFF" : "");

    if (currentState == OnOffState::ON) {
        onSetLevel(this->currentLevel); // trigger the actual level change
    }
}

inline void Potentiometer::setState(OnOffState state) {
    currentState = OnOffState::ON; // this will enable actual level set, needed to switch either ON or OFF

    if (state == OnOffState::ON) {
        logger.debug("switching ON");
        onSwitchOn();
    } else {
        logger.debug("switching OFF");
        onSwitchOff();
        currentState = OnOffState::OFF;
    }
}

inline void Potentiometer::toggleState() {
    setState(currentState == OnOffState::ON ? OnOffState::OFF : OnOffState::ON);
}

inline void Potentiometer::onSwitchOff() {
    setLevel(0.0);
}

inline void Potentiometer::onSwitchOn() {
    setLevel(this->currentLevel);
}

#endif
