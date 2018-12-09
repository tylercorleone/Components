#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "commons/components_commons.h"

class Component : public Loggable {
protected:
	Component();
	Component(const char *name, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);
};

inline Component::Component() {

}

inline Component::Component(const char *name, LogLevel logLevel) :
		Loggable(name, logLevel) {
}

#include "GenericDevice.h"

#include "components/Battery.h"
#include "components/Button.h"
#include "components/CappablePotentiometer.h"
#include "components/GradualCappablePotentiometerActuator.h"
#include "components/GradualLevelSetter.h"
#include "components/GradualPotentiometerActuator.h"
#include "components/LightnessDimmer.h"
#include "components/LithiumBattery.h"
#include "components/Potentiometer.h"

#endif
