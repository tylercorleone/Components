#ifndef STATE_H
#define STATE_H

#include "commons/components_commons.h"
#include "DeviceAware.h"
#include "events/EventHandler.h"

/**
 * A class that defines State's base behavior, that is event handling
 * and entering/exiting from that state. Extend State<D, E> to create
 * a custom state for your device
 */
class AbstractState {
	friend class GenericDevice;
	template<typename D, typename E> friend class State;
protected:
	Logger logger {nullptr, LogLevel::OFF};
	virtual bool onEnterState() = 0;
	virtual void onExitState() = 0;
	virtual ~AbstractState() {};
private:
	AbstractState(const char *stateName, LogLevel logLevel);
	virtual bool canHandleEvent(const Event &event) = 0;
	virtual bool handleGenericEvent(const Event &event) = 0;
	virtual bool onEnterStateWithGenericEvent(const Event &event) = 0;
};

/*
 * This is the base class that you should extend to create a device's state
 *
 * The behavior of the state is given by the following methods:
 *
 * 	bool handleEvent(const E &event)
 *	bool onEnterState()
 *	bool onEnterState(const E &event)
 *	void onExitState()
 *
 *	You can just customize the behavior you are interested in, e.g.
 *	if you are not interested in what the state does when entering
 *	with an event, you can leave the default method implementation
 */
template<typename D, typename E> class State : public AbstractState, public DeviceAware<D>, public EventHandler<E> {
	friend class GenericDevice;
protected:
	State(D &device, const char *stateName, LogLevel logLevel = COMPONENTS_DEFAULT_LOG_LEVEL);
	virtual bool handleEvent(const E &event) override {return false;}
	virtual bool onEnterState() {return false;}
	virtual bool onEnterState(const E &event) {return false;}
	virtual void onExitState() {}
private:
	bool canHandleEvent(const Event &event) override;
	bool handleGenericEvent(const Event &event) override;
	bool onEnterStateWithGenericEvent(const Event &event) override;
};

/*
 *******************
 * Implementations
 *******************
 */

inline AbstractState::AbstractState(const char *stateName, LogLevel logLevel) {
	logger.setName(stateName);
	logger.setLogLevel(logLevel);
}

template<typename D, typename E>
inline State<D, E>::State(D &device, const char *stateName, LogLevel logLevel) :
		AbstractState(stateName, logLevel), DeviceAware<D>::DeviceAware(device) {
}

template<typename D, typename E>
inline bool State<D, E>::canHandleEvent(const Event &event) {
	return EventHandler<E>::EventHandler::canHandleEvent(event);
}

template<typename D, typename E>
inline bool State<D, E>::handleGenericEvent(const Event &event) {
	return handleEvent(static_cast<const E&>(event));
}

template<typename D, typename E>
inline bool State<D, E>::onEnterStateWithGenericEvent(const Event &event) {
	return onEnterState(static_cast<const E&>(event));
}

#endif
