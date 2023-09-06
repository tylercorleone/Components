#include "components/Potentiometer.h"

Potentiometer::Potentiometer(const char *name,
                             LogLevel logLevel) :
        Component(name, logLevel) {
}

float Potentiometer::getLevel() { return currentLevel; }

void Potentiometer::setLevel(float level) {
    this->currentLevel = components_constrain(level, 0.0f, 1.0f);

    logger.trace("setting level %f%s", this->currentLevel, currentState == OnOffState::OFF ? " while OFF" : "");

    if (currentState == OnOffState::ON) {
        onSetLevel(this->currentLevel); // trigger the actual level change
    }
}

OnOffState Potentiometer::getState() { return currentState; }

void Potentiometer::setState(OnOffState state) {
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

void Potentiometer::toggleState() {
    setState(currentState == OnOffState::ON ? OnOffState::OFF : OnOffState::ON);
}

void Potentiometer::onSwitchOff() {
    setLevel(0.0);
}

void Potentiometer::onSwitchOn() {
    setLevel(this->currentLevel);
}
