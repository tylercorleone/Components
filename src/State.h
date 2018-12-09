#ifndef STATE_H
#define STATE_H

#include "commons/components_commons.h"
#include "DeviceAware.h"
#include "events/EventHandler.h"

class AbstractState : public Loggable {
	friend class GenericDevice;
	template<typename D, typename E> friend class State;
protected:
	virtual bool onEnterState() {return false;}
	virtual void onExitState() {}
	virtual ~AbstractState() {};
protected:
	AbstractState(const char *stateName = nullptr) : Loggable(stateName) {}
	virtual bool canHandleEvent(const Event &event) = 0;
	virtual bool handleGenericEvent(const Event &event) = 0;
	virtual bool onEnterStateWithGenericEvent(const Event &event) = 0;
};

template<typename D, typename E> class State : public AbstractState, public DeviceAware<D>, public EventHandler<E> {
	friend class GenericDevice;
protected:
	State(D &device, const char *stateName = nullptr) : AbstractState(stateName), DeviceAware<D>::DeviceAware(device) {}
	virtual bool onEnterState(const E &event) {return false;}
	virtual bool handleEvent(const E &event) override {return false;}
	virtual ~State() {};
protected:

	bool canHandleEvent(const Event &event) override {
		return EventHandler<E>::EventHandler::canHandleEvent(event);
	}

	bool handleGenericEvent(const Event &event) override {
		return handleEvent(static_cast<const E&>(event));
	}

	bool onEnterStateWithGenericEvent(const Event &event) override {
		return onEnterState(static_cast<const E&>(event));
	}
};

#endif
