#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H

#include "GenericEvent.h"
#include <stdint.h>

class ButtonEvent: public GenericEvent {
public:
	ButtonEvent(uint8_t clicksCount, uint8_t holdStepsCount);
	uint8_t getClicksCount() const;
	uint8_t getHoldStepsCount() const;
	virtual const char* getEventTypeHash() const override;
	static bool isEventInstanceOf(const GenericEvent &event);
	virtual ~ButtonEvent();
private:
	static const char* ButtonEventTypeHash();
	uint8_t clicksCount;
	uint8_t holdStepsCount;
};

inline ButtonEvent::ButtonEvent(uint8_t clicksCount, uint8_t holdStepsCount) :
		clicksCount(clicksCount), holdStepsCount(holdStepsCount) {
}

inline uint8_t ButtonEvent::getClicksCount() const {
	return clicksCount;
}

inline uint8_t ButtonEvent::getHoldStepsCount() const {
	return holdStepsCount;
}

inline const char* ButtonEvent::getEventTypeHash() const {
	return ButtonEvent::ButtonEventTypeHash();
}

inline bool ButtonEvent::isEventInstanceOf(const GenericEvent &event) {
	return ButtonEvent::ButtonEventTypeHash() == event.getEventTypeHash();
}

inline const char* ButtonEvent::ButtonEventTypeHash() {
	const static char staticVar = 'B';
	return &staticVar;
}

inline ButtonEvent::~ButtonEvent() {

}

#endif
