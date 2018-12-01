#ifndef LITHIUMBATTERY_H
#define LITHIUMBATTERY_H

#include "Battery.h"

class LithiumBattery: public Battery {
public:
	LithiumBattery(float voltageEmpty, float firstLinearStepEndVoltage,
			float voltageFull, float firstLinearStepEndCapacity,
			float (*readVoltageFunc)(void));
	float getRemainingCharge() const override;
	float getVoltageFull();
	float getVoltageEmpty();
private:
	float voltageFull;
	float voltageEmpty;
	float firstLinearStepEndVoltage;
	float firstLinearStepEndCapacity;
};

inline LithiumBattery::LithiumBattery(float voltageEmpty,
		float firstLinearStepEndVoltage, float voltageFull,
		float firstLinearStepEndCapacity, float (*readVoltageFunc)(void)) :
		Battery(readVoltageFunc) {
	this->voltageFull = voltageFull;
	this->voltageEmpty = voltageEmpty;
	this->firstLinearStepEndVoltage = firstLinearStepEndVoltage;
	this->firstLinearStepEndCapacity = firstLinearStepEndCapacity;
}

inline float LithiumBattery::getVoltageFull() {
	return voltageFull;
}

inline float LithiumBattery::getVoltageEmpty() {
	return voltageEmpty;
}

/**
 * An approximation of the charge/voltage function for Lithium batteries
 */
inline float LithiumBattery::getRemainingCharge() const {
	float currentVoltage = readVoltage();
	currentVoltage = _constrain(currentVoltage, voltageEmpty, voltageFull);

	if (currentVoltage < firstLinearStepEndCapacity) {

		/*
		 * low charge zone
		 */
		return (currentVoltage - voltageEmpty) * (firstLinearStepEndCapacity)
				/ (firstLinearStepEndVoltage - voltageEmpty);
	} else {

		return firstLinearStepEndCapacity
				+ (currentVoltage - firstLinearStepEndVoltage)
						* (1.0f - firstLinearStepEndCapacity)
						/ (voltageFull - firstLinearStepEndVoltage);
	}
}

#endif
