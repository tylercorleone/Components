#ifndef LOGGABLE_H
#define LOGGABLE_H

#ifndef LOGGABLE_DEFAULT_LOG_LEVEL
#define LOGGABLE_DEFAULT_LOG_LEVEL LogLevel::TRACE
#endif

enum class LogLevel {
	OFF, TRACE, DEBUG, INFO, WARNING, ERROR
};

class Loggable {
public:
	Loggable();
	Loggable(const char *name, LogLevel logLevel = LOGGABLE_DEFAULT_LOG_LEVEL);
	const char* getName() const;
	void setName(const char *instanceName);
	LogLevel getLogLevel() const;
	void setLogLevel(LogLevel logLevel);
private:
	const char *name = nullptr;
	LogLevel logLevel = LOGGABLE_DEFAULT_LOG_LEVEL;
};

inline Loggable::Loggable() {

}

inline Loggable::Loggable(const char *name, LogLevel logLevel) :
		name(name), logLevel(logLevel) {
}

inline const char* Loggable::getName() const {
	return name;
}

inline void Loggable::setName(const char *name) {
	this->name = name;
}

inline LogLevel Loggable::getLogLevel() const {
	return logLevel;
}

inline void Loggable::setLogLevel(LogLevel logLevel) {
	this->logLevel = logLevel;
}

#endif
