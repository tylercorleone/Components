#ifndef DEVICEAWARE_H
#define DEVICEAWARE_H

template<class D> class DeviceAware {
public:
	DeviceAware(D &device) :
			device(device) {
	}

	D& Device() {
		return device;
	}
private:
	D &device;
};

#endif
