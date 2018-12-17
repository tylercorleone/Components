#ifndef COMPONENTS_H
#define COMPONENTS_H

#include "commons/components_commons.h"
#include "GenericDevice.h"

class Component {
protected:
	Component() {}
	Component(const char *name, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL) {
		logger.setName(name);
		logger.setLogLevel(logLevel);
	}
	Logger logger {nullptr, LogLevel::OFF};
};

#include "components/Battery.h"
#include "components/Button.h"
#include "components/CappablePotentiometer.h"
#include "components/GradualCappablePotentiometerActuator.h"
#include "components/GradualLevelSetter.h"
#include "components/GradualPotentiometerActuator.h"
#include "components/LightnessDimmer.h"
#include "components/LithiumBattery.h"
#include "components/Potentiometer.h"
#include "components/SignalGenerator.h"

#endif
