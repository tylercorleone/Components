#ifndef DEVICE_AWARE_H
#define DEVICE_AWARE_H

template<typename D>
class DeviceAware {
public:
    DeviceAware(D &device) :
            device(device) {}

    D &getDevice() {
        return device;
    }

private:
    D &device;
};

#endif
