#ifndef BATTERY_H
#define BATTERY_H

class Battery {
public:
	Battery(float (*readVoltageFunc)()) :
			readVoltage(readVoltageFunc) {

	}
	/**
	 * Returns a value between 0.0 and 1.0
	 */
	virtual float getRemainingCharge() const = 0;
	virtual ~Battery() {
	}
protected:
	float (*readVoltage)(void);
};

#endif
