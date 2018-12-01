#ifndef NAMED_H
#define NAMED_H

class Named {
public:
	Named(const char *instanceName = nullptr) :
			instanceName(instanceName) {
	}

	const char* getName() {
		return instanceName;
	}

	void setName(const char *instanceName) {
		this->instanceName = instanceName;
	}

private:
	const char *instanceName;
};

#endif
