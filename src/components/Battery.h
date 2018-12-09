#ifndef BATTERY_H
#define BATTERY_H

#include "Components.h"

/**
 * Battery is a component that can be inspected to "getRemainingCharge"
 */
class Battery : public Component {
public:
	virtual float getRemainingCharge() const = 0;
	virtual ~Battery() {
	}
};

class GenericBattery: public Battery {
public:
	GenericBattery(float (*readRemainingChargeFunc)());
	float getRemainingCharge() const override;
protected:
	float (*readRemainingCharge)(void);
};

class LinearCapacityBattery: public Battery {
public:
	LinearCapacityBattery(float voltageEmpty, float voltageFull,
			float (*readVoltageFunc)());
	float getRemainingCharge() const override;
protected:
	float voltageEmpty;
	float voltageFull;
	float (*readVoltage)(void);
};

inline GenericBattery::GenericBattery(float (*readRemainingChargeFunc)()) :
		readRemainingCharge(readRemainingChargeFunc) {

}

inline float GenericBattery::getRemainingCharge() const {
	return readRemainingCharge();
}

inline LinearCapacityBattery::LinearCapacityBattery(float voltageEmpty,
		float voltageFull, float (*readVoltageFunc)()) :
		voltageEmpty(voltageEmpty), voltageFull(voltageFull), readVoltage(
				readVoltageFunc) {

}

inline float LinearCapacityBattery::getRemainingCharge() const {
	float _charge = (readVoltage() - voltageEmpty)
			/ (voltageFull - voltageEmpty);
	return _constrain(_charge, 0.0f, 1.0f);
}

#endif
